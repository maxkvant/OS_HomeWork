#include<lib.h>
#include <stdarg.h>
extern void* start64;

void backtraceF(uint64_t* frame) { //frame: %rbp
    printf("backtrace {\n");
    int max_stack_size = 1024;
    
    char* start64p = (char*)&start64;
    
    for (int i = 0; i < max_stack_size; i++) {
        printf("frame: %llx | returns_to (instruction): %llx\n", frame, *(frame+1));
        frame = (uint64_t*)*frame;
        if ((char*)*(frame + 1) == start64p + 0x22) {
            break;
        }
    } 
    printf("}\n");
} 
