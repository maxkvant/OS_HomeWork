#include<lib.h>
#include <stdarg.h>
extern void* start64ret;
extern void* bootstrap_stack_top;

void backtraceF(uint64_t* frame, uint64_t *rsp) { //frame: %rbp
    printf("backtrace {\n");
    
    while ( (uint64_t)frame >= (uint64_t)rsp && 
            *frame > (uint64_t)frame &&
            (uint64_t)(char*)*frame <= (uint64_t)(char*)&bootstrap_stack_top) {
        printf("    frame: %llx | returns_to (instruction): %llx\n", frame, *(frame+1));
        frame = (uint64_t*)*frame;
    }
    
    //if ((char*)*(frame + 1) != (char*)&start64ret) {
    //    printf("backtrace isn\'t correct %x != %x (start64 ret) !!!\n", *(frame + 1), &start64ret);
    //}
    printf("}\n");
} 
