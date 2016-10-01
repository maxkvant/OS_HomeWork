#include<lib.h>
#include <stdarg.h>
extern void* start64ret;

void backtraceF(uint64_t* frame, uint64_t *rsp) { //frame: %rbp
    printf("backtrace {\n");
    uint64_t max_stack_size = 1 << 20;
    
    for (int i = 0; (uint64_t)*frame - (uint64_t)rsp - 1 <= max_stack_size; i++) {
        printf("    frame: %llx | returns_to (instruction): %llx\n", frame, *(frame+1));
        frame = (uint64_t*)*frame;
    }
    
    //if ((char*)*(frame + 1) != (char*)&start64ret) {
    //    printf("backtrace isn\'t correct %x != %x (start64 ret) !!!\n", *(frame + 1), &start64ret);
    //}
    printf("}\n");
} 
