#include <stdint.h>
#include <memory.h>
#include <lib.h>
#include <desc.h>

static uint64_t idt[2 * 60];

extern void interrupt0(void);
extern void interrupt1(void);
extern void interrupt2(void);
extern void interrupt3(void);
extern void interrupt4(void);
extern void interrupt5(void);
extern void interrupt6(void);
extern void interrupt7(void);
extern void interrupt8(void);
extern void interrupt9(void);
extern void interrupt10(void);
extern void interrupt11(void);
extern void interrupt12(void);
extern void interrupt13(void);
extern void interrupt14(void);
extern void interrupt15(void);
extern void interrupt16(void);
extern void interrupt17(void);
extern void interrupt18(void);
extern void interrupt19(void);
extern void interrupt20(void);
extern void interrupt21(void);
extern void interrupt22(void);
extern void interrupt23(void);
extern void interrupt24(void);
extern void interrupt25(void);
extern void interrupt26(void);
extern void interrupt27(void);
extern void interrupt28(void);
extern void interrupt29(void);
extern void interrupt30(void);
extern void interrupt31(void);
extern void interrupt32(void);
extern void interrupt33(void);
extern void interrupt34(void);
extern void interrupt35(void);
extern void interrupt36(void);
extern void interrupt37(void);
extern void interrupt38(void);
extern void interrupt39(void);
extern void interrupt40(void);
extern void interrupt41(void);
extern void interrupt42(void);
extern void interrupt43(void);
extern void interrupt44(void);
extern void interrupt45(void);
extern void interrupt46(void);
extern void interrupt47(void);
extern void interrupt48(void);
extern void interrupt49(void);
extern void interrupt50(void);
extern void interrupt51(void);
extern void interrupt52(void);
extern void interrupt53(void);
extern void interrupt54(void);
extern void interrupt55(void);
extern void interrupt56(void);
extern void interrupt57(void);
extern void interrupt58(void);
extern void interrupt59(void);

static void setOne(uint64_t* a, uint64_t offset) {
    a[1] = offset >> 32;
    a[0] = offset & ((1 << 16) - 1);
    a[0] |= (offset & (((uint64_t)1 << 32) - (1 << 15))) << 32;
    a[0] |= ((uint64_t)1) << 47;
    a[0] |= ((uint64_t)14) << (8 + 32);
    a[0] |= KERNEL_CS << 16;
}

void initIdt() {
    setOne(&idt[0], (uint64_t)interrupt0);
    setOne(&idt[2], (uint64_t)interrupt1);
    setOne(&idt[4], (uint64_t)interrupt2);
    setOne(&idt[6], (uint64_t)interrupt3);
    setOne(&idt[8], (uint64_t)interrupt4);
    setOne(&idt[10], (uint64_t)interrupt5);
    setOne(&idt[12], (uint64_t)interrupt6);
    setOne(&idt[14], (uint64_t)interrupt7);
    setOne(&idt[16], (uint64_t)interrupt8);
    setOne(&idt[18], (uint64_t)interrupt9);
    setOne(&idt[20], (uint64_t)interrupt10);
    setOne(&idt[22], (uint64_t)interrupt11);
    setOne(&idt[24], (uint64_t)interrupt12);
    setOne(&idt[26], (uint64_t)interrupt13);
    setOne(&idt[28], (uint64_t)interrupt14);
    setOne(&idt[30], (uint64_t)interrupt15);
    setOne(&idt[32], (uint64_t)interrupt16);
    setOne(&idt[34], (uint64_t)interrupt17);
    setOne(&idt[36], (uint64_t)interrupt18);
    setOne(&idt[38], (uint64_t)interrupt19);
    setOne(&idt[40], (uint64_t)interrupt20);
    setOne(&idt[42], (uint64_t)interrupt21);
    setOne(&idt[44], (uint64_t)interrupt22);
    setOne(&idt[46], (uint64_t)interrupt23);
    setOne(&idt[48], (uint64_t)interrupt24);
    setOne(&idt[50], (uint64_t)interrupt25);
    setOne(&idt[52], (uint64_t)interrupt26);
    setOne(&idt[54], (uint64_t)interrupt27);
    setOne(&idt[56], (uint64_t)interrupt28);
    setOne(&idt[58], (uint64_t)interrupt29);
    setOne(&idt[60], (uint64_t)interrupt30);
    setOne(&idt[62], (uint64_t)interrupt31);
    setOne(&idt[64], (uint64_t)interrupt32);
    setOne(&idt[66], (uint64_t)interrupt33);
    setOne(&idt[68], (uint64_t)interrupt34);
    setOne(&idt[70], (uint64_t)interrupt35);
    setOne(&idt[72], (uint64_t)interrupt36);
    setOne(&idt[74], (uint64_t)interrupt37);
    setOne(&idt[76], (uint64_t)interrupt38);
    setOne(&idt[78], (uint64_t)interrupt39);
    setOne(&idt[80], (uint64_t)interrupt40);
    setOne(&idt[82], (uint64_t)interrupt41);
    setOne(&idt[84], (uint64_t)interrupt42);
    setOne(&idt[86], (uint64_t)interrupt43);
    setOne(&idt[88], (uint64_t)interrupt44);
    setOne(&idt[90], (uint64_t)interrupt45);
    setOne(&idt[92], (uint64_t)interrupt46);
    setOne(&idt[94], (uint64_t)interrupt47);
    setOne(&idt[96], (uint64_t)interrupt48);
    setOne(&idt[98], (uint64_t)interrupt49);
    setOne(&idt[100], (uint64_t)interrupt50);
    setOne(&idt[102], (uint64_t)interrupt51);
    setOne(&idt[104], (uint64_t)interrupt52);
    setOne(&idt[106], (uint64_t)interrupt53);
    setOne(&idt[108], (uint64_t)interrupt54);
    setOne(&idt[110], (uint64_t)interrupt55);
    setOne(&idt[112], (uint64_t)interrupt56);
    setOne(&idt[114], (uint64_t)interrupt57);
    setOne(&idt[116], (uint64_t)interrupt58);
    setOne(&idt[118], (uint64_t)interrupt59);

    struct desc_table_ptr ptr = {60 * 16 - 1, (uint64_t)idt};
    write_idtr(&ptr);
}

static uint32_t cnti = 0;

extern void backtrace();

void interruptF(uint64_t x) {
    char* s = "00 -- ";
    s[1] = x % 10 + '0'; s[0] = (x / 10) % 10 + '0';
    puts("iterrupt ");
    puts(s);
    char* s2 = "00000000\n";
    int i = 8;
    uint32_t cnt = cnti;
    while (i--) {
        s2[i] = cnt % 10 + '0';
        cnt /= 10;
    }
    cnti++;
    puts(s2);
    backtrace();
    if (32 <= x && x < 48) {
        picEndOfInterrupt((uint16_t)x);
    }
}



/*for gdb:
break interrupt0
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
*/
