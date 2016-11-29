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

#define TIMES 555555



void F1() {
    int k = 0;
    while (1) {
        if (k % TIMES == 0) {
            printf("thread F1 running\n");
        }
        k++;
    }
}

void F2() {
    int k = 0;
    while (1) {
        if (k % TIMES == 0) {
            printf("thread F2 running\n");
        }
        k++;
    }
}

void F3() {
    int k = 0;
    while (1) {
        if (k % TIMES == 0) {
            printf("thread F3 running\n");
        }
        k++;
    }
}



void H() {
    int64_t k = 10ll * TIMES;
    while(k--) {
        if (k % TIMES == 0) {
            printf("thread H running\n");
        }
    }
}

void G() {
    int64_t k = 10;
    while(k--) {
        printf("thread G running\n");
    }
}

void main(multiboot_info_t *mbt) {
    qemu_gdb_hang();
    
    memMapInit(mbt);
    
    serialSetup();
    initIdt();
    picSetup();
    pitSetup();
    
    enable_ints();
    
    start_thread(F1, 1);
    start_thread(F2, 1);
    start_thread(F3, 1);
    start_thread(G, 0);
    start_thread(H, 0);
    int k = 0;
    
    while (1) {
        if (k % TIMES == 0) {
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
break backtrace
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