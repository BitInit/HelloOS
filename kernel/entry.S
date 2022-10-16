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

.global ret_system_call
ret_system_call:
	movq	%rax,	0x80(%rsp)
	popq	%r15
	popq	%r14
	popq	%r13
	popq	%r12
	popq	%r11
	popq	%r10
	popq	%r9
	popq	%r8
	popq	%rbx
	popq	%rcx
	popq	%rdx
	popq	%rsi
	popq	%rdi				 	
	popq	%rbp
	popq	%rax
	movq	%rax,	%ds
	popq	%rax
	movq	%rax,	%es
	popq	%rax
	addq	$0x38,	%rsp
	.byte	0x48
	sysexit

.extern system_call_function
.global system_call
system_call:
	sti
	subq	$0x38,	%rsp
	cld;

	pushq	%rax;
	movq	%es,	%rax;
	pushq	%rax;
	movq	%ds,	%rax;
	pushq	%rax;
	xorq	%rax,	%rax;
	pushq	%rbp;
	pushq	%rdi;
	pushq	%rsi;
	pushq	%rdx;
	pushq	%rcx;
	pushq	%rbx;
	pushq	%r8;
	pushq	%r9;
	pushq	%r10;
	pushq	%r11;
	pushq	%r12;
	pushq	%r13;
	pushq	%r14;
	pushq	%r15;
	movq	$0x10,	%rdx;
	movq	%rdx,	%ds;
	movq	%rdx,	%es;
	movq	%rsp,	%rdi
	callq	system_call_function
	jmp		ret_system_call
