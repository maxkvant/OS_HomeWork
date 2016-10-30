#ifndef __LIB_H__
#define __LIB_H__

    #include <stdint.h>
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
    void pageAllocInit();
    char* getPage();
    void delPage(char *page);
    
    typedef struct PageListNode_t PageListNode;
    
    struct PageListNode_t {
        PageListNode *next;
        PageListNode *prev;
        char* pointer;
    };
    
    void blockAllockInit(PageListNode * list);
    char* blockAllock(PageListNode * list, int sz);
    void blockAllockClear(PageListNode *listPages);
    
#endif
