#include<lib.h>

void backtraceF(uint64_t* frame) { //frame: %rbp
    printf("backtrace {\n");
    while(1) {
        printf("frame: %llx | returns_to (instruction): %llx\n", frame, *(frame+1));
        if (*(frame + 1) <= 0x100695) {
            break;
        }
        frame = (uint64_t*)*frame;
    } 
    printf("}\n");
} 
