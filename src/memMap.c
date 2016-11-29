#include <lib.h>
#include <multiboot.h>

extern char bss_phys_begin[];
extern char bss_phys_end[];
extern char data_phys_begin[];
extern char data_phys_end[];
extern char text_phys_begin[];
extern char text_phys_end[];

#define MAX_SZ 100

multiboot_memory_map_t memMap[MAX_SZ + 6];
static int64_t mem_map_length;

int64_t getMemMapLen() {
    return mem_map_length;
}

multiboot_memory_map_t* getMemMap() {
    return memMap;
}


/*static void printMemMap__(multiboot_memory_map_t* mmap_addr, uint64_t mmap_length) {
    for (uint64_t i = 0; i < mmap_length; i++) {
        multiboot_memory_map_t* mmap = mmap_addr + i;
        printf("    memory range: %llx - %llx, len %llx,  type %d\n", mmap->addr, mmap->addr + mmap->len - 1, mmap->len, mmap->type);
    }
    printf("\n");
}*/

/*void printMemMap() {
    printMemMap__(memMap, mem_map_length);
}*/

static void init_mmap(multiboot_memory_map_t* mmap, int type, uint64_t l, uint64_t r) {
    mmap -> size = sizeof(multiboot_memory_map_t) - sizeof(uint32_t);
    mmap->len  = r - l + 1;
    mmap->addr = l;
    mmap->type = type;
}

static void useSeg(uint64_t l, uint64_t r) {
    for (int i = 0; i < mem_map_length; i++) {
        if (memMap[i].type == 2) {
            continue;
        }
        uint64_t li = memMap[i].addr;
        uint64_t ri = li + memMap[i].len - 1;
        if (li <= l && r <= ri) {
            init_mmap(memMap + i, 1, li, l - 1);
            init_mmap(memMap + mem_map_length, 2, l, r);
            mem_map_length++;
            init_mmap(memMap + mem_map_length, 1, r+1, ri);
            mem_map_length++;
            break;
        }
    }
}

void memMapInit(multiboot_info_t *mbt) {
    //printf(" bss_phys: [%llx %llx)\n", (uint64_t)bss_phys_begin , (uint64_t)bss_phys_end);
    //printf("text/data_phys: [%llx %llx)\n", (uint64_t)text_phys_begin , (uint64_t)data_phys_end);
    //printf("mbt %llx\n", mbt);
    
    mem_map_length = 0;
    multiboot_memory_map_t* mmap = (multiboot_memory_map_t*)(uint64_t)(mbt->mmap_addr);
    while((uint64_t)mmap <= (uint64_t)mbt->mmap_addr + (uint64_t)mbt->mmap_length - 1) {
        memMap[mem_map_length] = *mmap;
        mmap = (multiboot_memory_map_t*)((uint64_t)mmap + mmap->size + sizeof(uint32_t));
        mem_map_length++;
        if (mem_map_length > MAX_SZ) {
            printf("mem_map too large");
        }
    }
    
    
    //printf("default mem_map:\n");
    //printMemMap();
    
    //printf("new mem_map:\n");
    useSeg((uint64_t)text_phys_begin, (uint64_t)bss_phys_end - 1);
    //printMemMap();
}