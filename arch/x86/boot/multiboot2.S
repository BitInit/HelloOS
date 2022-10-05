#define ASM_FILE 1
#include "multiboot2.h"

#ifdef KERN_VBE_MODE
#define FRAMEBUFFER_WIDTH 1024
#define FRAMEBUFFER_HEIGHT 768
#define FRAMEBUFFER_BPP 24
#endif /* KERN_VBE_MODE */

.code32
.section .bs.text
.globl _start
_start:
    jmp multiboot_entry

.align 8
multiboot_header:
    .long MULTIBOOT2_HEADER_MAGIC                  # magic number (multiboot 2)
    .long MULTIBOOT_ARCHITECTURE_I386              # architecture 0 (protected mode i386)
    .long multiboot_header_end - multiboot_header  # header length
    # checksum
    .long -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + (multiboot_header_end - multiboot_header))
    # insert optional multiboot tags here

#ifdef KERN_VBE_MODE
    .align 8
    tag_framebuffer:
        .short MULTIBOOT_HEADER_TAG_FRAMEBUFFER
        .short MULTIBOOT_HEADER_TAG_OPTIONAL
        .long  tag_framebuffer_end - tag_framebuffer
        .long  FRAMEBUFFER_WIDTH    # screen width
        .long  FRAMEBUFFER_HEIGHT   # screen height
        .long  FRAMEBUFFER_BPP      # bit per pixel
    tag_framebuffer_end:
#endif /* KERN_VBE_MODE */

    # required end tag
    .align 8
    .short MULTIBOOT_HEADER_TAG_END # type
    .short 0                        # flags
    .long  8                        # size
multiboot_header_end:

multiboot_entry:
    cli
    # the magic value
    mov %eax, mb2_magic
    # the pointer to the Multiboot information structure
    mov %ebx, mb2_info

    # 允许 PAE
    mov %cr4, %eax
    or $(1<<5), %eax
    mov %eax, %cr4

    /* 设置临时页表 */
    # 顶级页目录表
    mov $pml4, %eax
    mov $pdpt, %ebx
    or $0x03, %ebx
    mov %ebx, 0(%eax)
    mov %ebx, 2048(%eax) /* 一个页地址占8B，则为 256*8 */
    # 页目录指针表
    mov $pdpt, %eax
    mov $pd, %ebx
    or $0x03, %ebx
    mov %ebx, 0(%eax)
    # 页目录表
    mov $512, %ecx
    mov $pd, %eax
    mov $0x83, %ebx
.pd_loop:
    mov %ebx, 0(%eax)
    add $0x200000, %ebx
    add $0x08, %eax
    loop .pd_loop

    # 设置 CR3
    mov $pml4, %eax
    mov %eax, %cr3

    # 切换到 long 模式
    mov $0xC0000080, %ecx
    rdmsr
    or $(1<<8), %eax
    wrmsr

    # 开启分页
    mov %cr0, %eax
    or $(1<<31), %eax
    mov %eax, %cr0

    # 重新设置 GDT
    mov $gdt64_pointer64, %eax
    lgdt 0(%eax)
    # 跳转到 64 位代码执行
    jmp $0x8, $_start64
    hlt
    ret

.extern kernel_start
.code64
go_to_kernel:
    .quad kernel_start
_start64:
    # 配置 IDT
    lidt idt_pointer64(%rip)
setup_IDT:							
	leaq	ignore_int(%rip),	%rdx
	movq	$(0x08 << 16),	%rax
	movw	%dx,	%ax
	movq	$(0x8E00 << 32),	%rcx
	addq	%rcx,	%rax
	movl	%edx,	%ecx
	shrl	$16,	%ecx
	shlq	$48,	%rcx
	addq	%rcx,	%rax
	shrq	$32,	%rdx
	leaq	IDT_Table(%rip),	%rdi
	mov	$256,	%rcx
rp_sidt:
	movq	%rax,	(%rdi)
	movq	%rdx,	8(%rdi)
	addq	$0x10,	%rdi
	dec	%rcx
	jne	rp_sidt

    # 更新
    mov $0x10, %rax
    mov %rax, %ds
    mov %rax, %es
    mov %rax, %fs
    mov %rax, %gs
    mov %rax, %ss
    # 设置栈地址
    mov _stack_start(%rip), %rsp

SetUp_TSS64:
    leaq	TSS64_Table(%rip),	%rdx
	xorq	%rax,	%rax
	xorq	%rcx,	%rcx
	movq	$0x89,	%rax
	shlq	$40,	%rax
	movl	%edx,	%ecx
	shrl	$24,	%ecx
	shlq	$56,	%rcx
	addq	%rcx,	%rax
	xorq	%rcx,	%rcx
	movl	%edx,	%ecx
	andl	$0xffffff,	%ecx
	shlq	$16,	%rcx
	addq	%rcx,	%rax
	addq	$103,	%rax
	leaq	GDT_Table(%rip),	%rdi
	movq	%rax,	80(%rdi)	//tss segment offset
	shrq	$32,	%rdx
	movq	%rdx,	88(%rdi)	//tss+1 segment offset

    movq go_to_kernel(%rip), %rax
    pushq $0x08
    pushq %rax
    lretq

.extern IRQ_nothing
go_to_irp_nothing:
    .quad IRQ_nothing
ignore_int:
	cld
	pushq	%rax
	pushq	%rbx
	pushq	%rcx
	pushq	%rdx
	pushq	%rbp
	pushq	%rdi
	pushq	%rsi

	pushq	%r8
	pushq	%r9
	pushq	%r10
	pushq	%r11
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15

	movq	%es,	%rax
	pushq	%rax
	movq	%ds,	%rax
	pushq	%rax

	movq	$0x10,	%rax
	movq	%rax,	%ds
	movq	%rax,	%es

	movq    go_to_irp_nothing(%rip), %rax
    pushq   $0x08
    pushq   %rax
    lretq

/*
 * 临时页表，4KB/页
 */
.section .bs.data
.align 0x1000
.global pml4
pml4: /* 顶级页目录表 */
    .skip 0x1000
pdpt: /* 页目录指针表*/
    .skip 0x1000
pd: /* 页目录表 */
    .skip 0x1000

.align 16
.globl GDT_Table
GDT_Table:
    .quad	0x0000000000000000			/*0	NULL descriptor		       	00*/
	.quad	0x0020980000000000			/*1	KERNEL	Code	64-bit	Segment	08*/
	.quad	0x0000920000000000			/*2	KERNEL	Data	64-bit	Segment	10*/
	.quad	0x0000000000000000			/*3	USER	Code	32-bit	Segment 18*/
	.quad	0x0000000000000000			/*4	USER	Data	32-bit	Segment 20*/
	.quad	0x0020f80000000000			/*5	USER	Code	64-bit	Segment	28*/
	.quad	0x0000f20000000000			/*6	USER	Data	64-bit	Segment	30*/
	.quad	0x00cf9a000000ffff			/*7	KERNEL	Code	32-bit	Segment	38*/
	.quad	0x00cf92000000ffff			/*8	KERNEL	Data	32-bit	Segment	40*/
	.fill	10,8,0					    /*10 ~ 11 TSS (jmp one segment <9>) in long-mode 128-bit 50*/
gdt64_pointer64:
    .short gdt64_pointer64-GDT_Table-1
    .quad GDT_Table

.global IDT_Table
IDT_Table:
    .fill 512, 8, 0
idt_pointer64:
    .short idt_pointer64-IDT_Table-1
    .quad IDT_Table

.global TSS64_Table
TSS64_Table:
    .fill 13, 8, 0
TSS64_END:

.globl mb2_magic
mb2_magic: .quad 0x00
.globl mb2_info
mb2_info: .quad 0x00

_stack_start:
    .quad init_task_union + 32768
