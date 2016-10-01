fS = open('idt1.S', 'w');
fc = open('idt2.c', 'w');

n = 60

fS.write(".text\n\n");

fS.write("    .globl doInterrupt\n")
fS.write("    .type doInterrupt, @function" + '\n');   
fS.write("doInterrupt:\n");
fS.write("   int $0\n");
fS.write("   ret\n\n");

regs = ["rax", "rbx", "rcx", "rdx", "rbp", "rdi", "rsi"];
for i in range(9, 16):
    regs.append("r" + str(i));
 
names = []
for i in range(n):
    names.append("interrupt" + str(i));
    
for i in range(n):
    name = names[i]
    fS.write("    .globl " + name + '\n');
    fS.write("    .type " + name + ", @function" + '\n');   
    fS.write(name  + ':\n');
    
    if (i == 8) or (10 <= i <= 14) or (i == 17):
        fS.write("    addq $8, %rsp\n\n");
    
    fS.write("    pushq %rbp\n");
    fS.write("    movq %rsp, %rbp\n\n");
    
    for reg in regs:
        fS.write("    pushq %" + reg + '\n');
    fS.write('\n');
    
    fS.write("    movq $" + str(i) + ", %rdi\n");
	
    fS.write("    call interruptF\n\n");
    
    for reg in reversed(regs):
        fS.write("    popq %" + reg + '\n');
    
    
    fS.write("    popq %rbp\n");
    fS.write("    iretq" + '\n\n\n');
    
fS.close()



fc.write("#include <stdint.h>\n");
fc.write("#include <memory.h>\n");
fc.write("#include <lib.h>\n");
fc.write("#include <desc.h>\n\n");

fc.write("static uint64_t idt[2 * " + str(n) + "];\n\n");

for name in names:
    fc.write("extern void " + name + "(void);" + '\n')
    


fc.write('\n');

fc.write("static void setOne(uint64_t* a, uint64_t offset) {\n");
fc.write("    a[1] = offset >> 32;\n");
fc.write("    a[0] = offset & ((1 << 16) - 1);\n");
fc.write("    a[0] |= (offset & (((uint64_t)1 << 32) - (1 << 15))) << 32;\n");
fc.write("    a[0] |= ((uint64_t)1) << 47;\n");
fc.write("    a[0] |= ((uint64_t)14) << (8 + 32);\n");
fc.write("    a[0] |= KERNEL_CS << 16;\n");
fc.write("}\n\n");


fc.write("void initIdt() {\n");

i = 0
for name in names:
    fc.write("    setOne(&idt[" + str(i) + "], (uint64_t)" + name + ");\n");
    i += 2;

fc.write("\n");
fc.write("    struct desc_table_ptr ptr = {" + str(n)  + " * 16 - 1, (uint64_t)idt};\n");
fc.write("    write_idtr(&ptr);\n");
fc.write("}\n\n");

fc.write("static uint32_t cnti = 0;\n\n");
fc.write("extern void backtrace();\n\n");

fc.write("void interruptF(uint64_t x) {\n");
fc.write("    char* s = \"00 -- \";\n");
fc.write("    s[1] = x % 10 + '0'; s[0] = (x / 10) % 10 + '0';\n");
fc.write("    puts(\"iterrupt \");\n");
fc.write("    puts(s);\n");
fc.write("    char* s2 = \"00000000\\n\";\n");
fc.write("    int i = 8;\n");
fc.write("    uint32_t cnt = cnti;\n");
fc.write("    while (i--) {\n");
fc.write("        s2[i] = cnt % 10 + '0';\n");
fc.write("        cnt /= 10;\n");
fc.write("    }\n");
fc.write("    cnti++;\n");
fc.write("    puts(s2);\n");
fc.write("    backtrace();\n");
fc.write("    if (32 <= x && x < 48) {\n");
fc.write("        picEndOfInterrupt((uint16_t)x);\n");
fc.write("    }\n");
fc.write("}\n\n");

fc.write("\n\n");

fc.write("/*for gdb:\n");
for name in names:
    fc.write("break " + name + '\n');
fc.write("*/\n");
