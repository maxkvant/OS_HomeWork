static void qemu_gdb_hang(void)
{
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}

#include <lib.h>
#include <ints.h>
#include <ioport.h>
#include <memory.h>

void main(multiboot_info_t *mbt) {
    qemu_gdb_hang();
    
    memMapInit(mbt);
    pageAllocInit();
    
    serialSetup();
    initIdt();
    
    
    for (int i = 0; i < 100; i++) {
        char* curPage = getPage();
        
        curPage[PAGE_SIZE - 1] = 'a';
        curPage[0] = 'b';
        curPage[100] = 'c';
        printf("%d: %llx, curPage[0] == %c ('c')\n", i, curPage, curPage[100]);
        delPage(curPage);
    }
    
    //picSetup();
    //enable_ints();
    //pitSetup();
    
    
    int k = 0;
    while (1) {
        if (k % 555555 == 0) {
        //    printf("running\n");
        }
        k++;
    }
}

/*
 * for gdb:
   gdb kernel
   set architecture i386:x86-64
   target remote localhost:1234
   set var wait=0
break backtrace
break backtraceF
break interrupt1
break interrupt2
break interrupt3
break interrupt4
break interrupt5
break interrupt6
break interrupt7
break interrupt8
break interrupt9
break interrupt10
break interrupt11
break interrupt12
break interrupt13
break interrupt14
break interrupt15
break interrupt16
break interrupt17
break interrupt18
break interrupt19
break interrupt20
break interrupt21
break interrupt22
break interrupt23
break interrupt24
break interrupt25
break interrupt26
break interrupt27
break interrupt28
break interrupt29
break interrupt30
break interrupt31
break interrupt32
break interrupt33
break interrupt34
break interrupt35
break interrupt36
break interrupt37
break interrupt38
break interrupt39
break interrupt40
break interrupt41
break interrupt42
break interrupt43
break interrupt44
break interrupt45
break interrupt46
break interrupt47
break interrupt48
break interrupt49
break interrupt50
break interrupt51
break interrupt52
break interrupt53
break interrupt54
break interrupt55
break interrupt56
break interrupt57
break interrupt58
break interrupt59
80000000000

   print (void*)*((uint64_t*)$rsp - 11)
   print (void*)*((uint64_t*)$rsp - 10)
   print (void*)*((uint64_t*)$rsp - 9)
   print (void*)*((uint64_t*)$rsp - 8)
   print (void*)*((uint64_t*)$rsp - 7)
   print (void*)*((uint64_t*)$rsp - 6)
   print (void*)*((uint64_t*)$rsp - 5)
   print (void*)*((uint64_t*)$rsp - 4)
   print (void*)*((uint64_t*)$rsp - 3)
   print (void*)*((uint64_t*)$rsp - 2)
   print (void*)*((uint64_t*)$rsp - 1)   
   print (void*)*((uint64_t*)$rsp + 0)
   print (void*)*((uint64_t*)$rsp + 1)
   print (void*)*((uint64_t*)$rsp + 2)
   print (void*)*((uint64_t*)$rsp + 3)
   print (void*)*((uint64_t*)$rsp + 4)
   print (void*)*((uint64_t*)$rsp + 5)
   print (void*)*((uint64_t*)$rsp + 6)
   print (void*)*((uint64_t*)$rsp + 7)
   print (void*)*((uint64_t*)$rsp + 8)
   print (void*)*((uint64_t*)$rsp + 9)
   print (void*)*((uint64_t*)$rsp + 10)
   print (void*)*((uint64_t*)$rsp + 11)
   print (void*)*((uint64_t*)$rsp + 12)
   print (void*)*((uint64_t*)$rsp + 13)
   print (void*)*((uint64_t*)$rsp + 14)
   print (void*)*((uint64_t*)$rsp + 15)
   print (void*)*((uint64_t*)$rsp + 16)
   print (void*)*((uint64_t*)$rsp + 17)
   print (void*)*((uint64_t*)$rsp + 18)
   print (void*)*((uint64_t*)$rsp + 19)
   print (void*)*((uint64_t*)$rsp + 20)   
   print $rsp
   print $rbp
   print $rip
   
   next
   
*/