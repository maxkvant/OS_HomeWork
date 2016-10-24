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
    node->next->prev = node->prev;
    node->prev->next = node->next;
    return node;
}


char* getPage();
static int pageForNodesSz = 0;
static char* pageForNodes;
uint8_t inGet = 0;

static Node* newNode() {
    if (pageForNodesSz < (int)sizeof(Node)) {
        pageForNodes = 0;
        //printf("try new page for nodes\n");
        if (order[0].next != &order[0]) {
            pageForNodes = remove(order[0].next)->pointer;
        }
        if (!pageForNodes) {
            //printf("can't alloc new Node\n");
            return 0;
        } else {
            pageForNodesSz = PAGE_SIZE;
        }
        //printf("new page for nodes %llx\n", pageForNodes);
    }
    pageForNodesSz -= sizeof(Node);
    Node* res = (Node*)(uint64_t)(pageForNodes + pageForNodesSz);
    //printf("pageForNodesSz = %d\n", pageForNodesSz);
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
    //printf("ii == %d\n\n", i);
    if (i >= ORDER_SZ) {
        if (order[i].next == &order[i]) {
            //printf("can't alloc new page\n");
            return 0;
        } else {
            return remove(order[0].next)->pointer;
        }
    } else {
        inGet = 1;
        //printf("remove(%llx);\n", order[i].next);
        Node *node = remove(order[i].next);
        for (; i > 0; i--) {
            Node *b = newNode();
            b->pointer = (node->pointer) + ((uint64_t)1 << (i - 1));
            //printf("add_%d %llx %llx\n", i, order + (i - 1), b);
            add(order + (i - 1), b);
        }
        inGet = 0;
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
        //printf("i = %d\n", i);
        if (first) {
            pageForNodes = (char*)L;
            pageForNodesSz = PAGE_SIZE;
            L += PAGE_SIZE;
            len -= PAGE_SIZE;
            first = 0;
        }
        for (int i = 48; i >= 0; i--) {
            int64_t curLen = ((int64_t)1 << (i + PAGE_SIZE_LOG));
            if (len > curLen) {
                Node *node = newNode();
                node->pointer = (char*)L;
                //printf("  i = %d: %llx - %llx\n", i, L, L + curLen - 1);
                add(order + i, node);
                L += curLen;
                len -= curLen;
            }
        }
    }
    //printf("!!                                                  sum_mem = %d\n", sum_mem);
}

void delPage(char *page) {
    Node* node = newNode();
    node->pointer = page;
    add(&order[0], node);
}