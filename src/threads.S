.text

    .globl switch_threads
    .type switch_threads, @function
switch_threads:
    pushq %rbx
    pushq %rbp
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    pushf
    
    movq %rsp, (%rdi)
    movq %rsi, %rsp
    
    popfq
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbp
    popq %rbx
    
    ret
    
    .globl init_stack
    .type init_stack, @function
init_stack: 
    movq %rsp, %rdx
    
    movq %rdi, %rsp
    
    pushq %rsi
    
    pushq %rbx
    pushq %rbp
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    pushf
    
    movq %rsp, %rax
    
    movq %rdx, %rsp
    
    ret
    
    .globl switch_threads_sti
    .type switch_threads_sti, @function
switch_threads_sti:
    pushq %rbx
    pushq %rbp
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    pushf
    
    movq %rsp, (%rdi)
    movq %rsi, %rsp
    
    popfq
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbp
    popq %rbx
    
    sti
    
    ret