#ifndef __LIB_H__
#define __LIB_H__

    #include <stdint.h>
    #include <ints.h>
    #include <multiboot.h>
    
    void serialSetup();
    void putc(char c);
    void puts(char *s);
    void putHex(uint64_t x);
    void initIdt();
    void picEndOfInterrupt(uint16_t);
    void picSetup();
    void pitSetup();
    int64_t sprintf(char *__buf, int64_t n, char* s, ...);
    int64_t printf(char* s, ...);
    void memMapInit(multiboot_info_t *mbt);
    
    void start_thread(void (*f)(), int is_daemon);
    void changeThread();
    void lock(int *);
    
    #define lock_ints disable_ints
    #define unlock_ints enable_ints
#endif
