
.code64
.text
.global ret_from_intr
ret_from_intr:
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rbx
    popq %rcx
    popq %rdx
    popq %rsi
    popq %rdi
    popq %rbp

    popq %rax
    movq %rax, %ds

    popq %rax
    movq %rax, %es

    popq %rax
    addq $0x10, %rsp
    
    sti
    iretq