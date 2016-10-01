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

extern void doInterrupt(void);
extern void backtrace(void);


int h(int x) {
    backtrace();
    int hh = x;
    return hh;
}

int g(int x) {
    int gg = h(x + 1);
    return gg;
}

int f(int x) {
    int ff = g(x+1);
    return ff;
}

char s1[2000];

void main(void) {
    qemu_gdb_hang();
    serialSetup();
    initIdt();
    picSetup();
    
    puts("before int\n");
        
    //doInterrupt();
    
    puts("after int\n");
    
    enable_ints();
    pitSetup();
    
    f(10);
    
    puts("while (1);");
    
    
    printf("printf test:\n");
            
            char *s2, *s3;
            printf(s2 = "    %x %x %x %x %x %x %x %x %x %x\n    %x %x %x %x %x %x %x %x %x %x\n    %x %x %x %x %x %x %x %x %x %x\n    %x %x %x %x %x %x %x %x %x %x\n    %x %x %x %x %x %x %x %x %x %x\n\n", 
                   0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
                   0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                   0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                   0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
                   0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
            
            
            
            sprintf(s1, 0xff,s3 = "    %x %x %x %x %x %x %x %x %x %x\n    %x %x %x %x %x %x %x %x %x %x\n    %x %x %x %x %x %x %x %x %x %x\n    %x %x %x %x %x %x %x %x %x %x\n    %x %x %x %x %x %x %x %x %x %x\n", 
                   0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
                   0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                   0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                   0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
                   0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
            
            printf("%s\n", s1);
            
            uint64_t a = ((uint64_t)-1) >> 1;
            printf("    %d %ld %lld %hd %hhi\n", a, a, a, a, a);
            printf("    %i %li %lli %hi %hhi\n", a, a, a, a, a);
            printf("    %x %lx %llx %hx %hhx\n", a, a, a, a, a);
            printf("    %u %lu %llu %hu %hhu\n", a, a, a, a, a);
            printf("    %o %lo %llo %ho %hho\n", a, a, a, a, a);
            printf("\n");
            
            a = 0x3c3c3c3c3c3c3c3cL;
            
            printf("    %d %ld %lld %hd %hhi\n", a, a, a, a, a);
            printf("    %i %li %lli %hi %hhi\n", a, a, a, a, a);
            printf("    %x %lx %llx %hx %hhx\n", a, a, a, a, a);
            printf("    %u %lu %llu %hu %hhu\n", a, a, a, a, a);
            printf("    %o %lo %llo %ho %hho\n", a, a, a, a, a);
            
            printf("\n");
            
            int m = sprintf(s1, 22, "%%H%c%clo, World! ---%d-----------------", 'e', 'l', 12345);
            printf("    %d\n", m);
            printf("    %s\n", s1);
            
            printf("\n");
    
    int k = 0;
    while (1) {
        if (k % 555555 == 0) {
            printf("running\n");
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
break f
break g
break h
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