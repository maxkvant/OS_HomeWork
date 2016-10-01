#include<lib.h>
#include <stdarg.h>
extern void* start64ret;

void backtraceF(uint64_t* frame) { //frame: %rbp
    printf("backtrace {\n");
    int max_stack_size = 1024;
    
    char* start64ret_p = (char*)&start64ret;
    
    for (int i = 0; i < max_stack_size; i++) {
        if (i > 2) {
            printf("frame: %llx | returns_to (instruction): %llx\n", frame, *(frame+1));
        }
        frame = (uint64_t*)*frame;
        if ((char*)*(frame + 1) == start64ret_p) {
            break;
        }
    } 
    printf("frame: %llx | returns_to (instruction): %llx\n", frame, *(frame+1));
    printf("}\n");
} 
