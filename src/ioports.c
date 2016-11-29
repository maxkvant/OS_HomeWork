#include <ioport.h>
#define serial 0x3f8

void serialSetup() {
    
    out8(serial + 1, 0);
    
    out8(serial + 3, (1 << 7));
    
    out8(serial + 0, 1);
    out8(serial + 1, 0);
    
    out8(serial + 3, 3);
}

void putc(char c) {
    while(((in8(serial + 5) >> 5) & 1) == 0) {}
    out8(serial + 0, c);
}

void puts(char *s) {
    while (*s) {
        putc(*s);
        s++;
        
    }
}

#define MASTER_C 0x20
#define MASTER_D 0x21
#define SLAVE_C 0xA0
#define SLAVE_D 0xA1

void picSetup() {
    out8(MASTER_C, 0x11);
    out8(MASTER_D, 32);
    out8(MASTER_D, (1 << 2));
    out8(MASTER_D, 1);
    
    out8( SLAVE_C, 0x11);
    out8( SLAVE_D, 40);
    out8( SLAVE_D, 2);
    out8( SLAVE_D, 1);
    
    
    out8(MASTER_D, 0xfe);
    out8( SLAVE_D, 0xff);  
}

void picEndOfInterrupt(uint16_t i) {
    if (i >= 40) {
    //    puts("EOI_S\n");
        out8( SLAVE_C, 1 << 5);
    }
    //puts("EOI_M\n");
    out8(MASTER_C, 1 << 5);
}

#define PIT_C 0x43
#define PIT_D 0x40

void pitSetup() {
    out8(PIT_C, 0x34);
    out8(PIT_D, 0xff);
    out8(PIT_D, 0xff);
    out8(MASTER_D, 0xfe);
}