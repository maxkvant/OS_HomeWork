#include <lib.h>
#include <memory.h>
#include <multiboot.h>

struct Node_t;
typedef struct Node_t Node;

struct Node_t {
    Node *next;
    Node *prev;
    char* pointer;
};

#define ORDER_SZ 50
static Node order[ORDER_SZ];
static Node deletedDescs;

static Node* init(Node *node) {
    node->next = node;
    node->prev = node;
    return node;
}

static void add(Node *a, Node *b) {
    Node *next = a->next;
    
    b->next = next;
    a->next = b;
    
    b->prev = a;
    next->prev = b;
}

static Node* remove(Node *node) {
    Node *next = node->next;
    Node *prev = node->prev;
    
    next->prev = node->prev;
    prev->next = node->next;
    
    return node;
}


char* getPage();
static int pageForNodesSz = 0;
static char* pageForNodes;

static Node* newNode() {
    if (deletedDescs.next != &deletedDescs) {
        return remove(deletedDescs.next);
    } 
    if (pageForNodesSz < (int)sizeof(Node)) {
        pageForNodes = 0;
        if (order[0].next != &order[0]) {
            pageForNodes = remove(order[0].next)->pointer;
        }
        if (!pageForNodes) {
            printf("can't alloc new Node\n");
            return 0;
        } else {
            pageForNodesSz = PAGE_SIZE;
        }
    }
    pageForNodesSz -= sizeof(Node);
    Node* res = (Node*)(uint64_t)(pageForNodes + pageForNodesSz);
    init(res);
    return res;
}

char* getPage() {
    int i;
    for (i = 1; i < ORDER_SZ; i++) {
        if (order[i].next != &order[i]) {
            break;
        }
    }
    if (i >= ORDER_SZ) {
        if (order[0].next != &order[0]) {
            Node *node = remove(order[0].next);
            add(&deletedDescs, node);
            return node->pointer;
        } else {
            printf("can't alloc new Page\n");
            return 0;
        }
    } else {
        Node *node = remove(order[i].next);
        for (; i > 0; i--) {
            Node *b = newNode();
            b->pointer = (node->pointer) + ((uint64_t)1 << (PAGE_SIZE_LOG + i - 1));
            add(order + (i - 1), b);
        }
        add(&deletedDescs, node);
        return node->pointer;
    }
}

int64_t getMemMapLen();
multiboot_memory_map_t* getMemMap();

static uint64_t ceil(uint64_t a) {
    if (a == 0) {
        return PAGE_SIZE;
    }
    return ((a - 1) | (PAGE_SIZE - 1)) + 1;
}

void pageAllocInit() {
    int64_t mem_map_len = getMemMapLen();
    multiboot_memory_map_t* memMap = getMemMap();
    
    for (int i = 0; i < ORDER_SZ; i++) {
        init(order + i);
    }
    
    uint64_t sum_mem = 0;
    init(&deletedDescs);
    
    for (int i = 0, first = 1; i < mem_map_len; i++) {
        if (memMap[i].type != 1) {
            continue;
        }
        
        uint64_t L = ceil(memMap[i].addr);
        int64_t len = memMap[i].len - (L - memMap[i].addr);
        sum_mem += len;
        if (len < PAGE_SIZE) {
            continue;
        }
        if (first) {
            pageForNodes = (char*)L;
            pageForNodesSz = PAGE_SIZE;
            L += PAGE_SIZE;
            len -= PAGE_SIZE;
            first = 0;
        }
        for (int i = 48; i >= 0; i--) {
            int64_t curLen = ((int64_t)1 << (i + PAGE_SIZE_LOG));
            if (len >= curLen) {
                Node *node = newNode();
                node->pointer = (char*)L;
                add(order + i, node);
                L += curLen;
                len -= curLen;
            }
        }
    }
}

void delPage(char *page) {
    Node* node = newNode();
    node->pointer = page;
    add(&order[0], node);
}