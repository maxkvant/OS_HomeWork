#include<lib.h>

void backtraceF(uint64_t* frame) { //frame: %rbp
    puts("backtrace {\n");
    while(1) {
        puts("    frame: "); putHex((uint64_t)frame); 
        puts(" | returns_to (instruction): "); putHex(*(frame + 1));
        puts("\n");
        if (*(frame + 1) <= 0x100695) {
            break;
        }
        frame = (uint64_t*)*frame;
    } 
    puts("}\n");
} 
