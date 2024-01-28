#include <os/linkage.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/segment.h>

.code32
.section .text.head
.global startup_64
.extern print
startup_64:
	/* 开启 PAE */
	movl 	%cr4, %eax
	btsl	$5, %eax
	movl	%eax, %cr4

	/* 设置顶级页目录表 */
	movl	$(init_level4_pgt - __START_KERNEL_map), %eax
	movl	%eax, %cr3

	/* 开启 long 模式 */
	mov $0xC0000080, %ecx
    rdmsr
    or $(1<<8), %eax
    wrmsr

	/* 开启分页 */
    mov %cr0, %eax
    or $(1<<31), %eax
    mov %eax, %cr0

	movl	$head_cpu_gdt_descr, %eax
	lgdt	0(%eax)
	jmp		$__KERNEL_CS, $secondary_startup_64

.code64
secondary_startup_64:
	/* 设置初始任务栈 */
	movq init_rsp(%rip),%rsp

	pushq $0
	popfq

	/* 
	 * 重新设置 gdt descriptor，
	 * 因为后续低地址空间是用户空间
	 */
	lgdt	cpu_gdt_descr(%rip)

	mov 	$__KERNEL_DS,	%ax
	mov 	%ax,	%ds
	mov 	%ax,	%ss
	mov 	%ax,	%es
	mov 	%ax,	%fs
	mov 	%ax,	%gs

.extern x86_64_start_kernel
	movq	$x86_64_start_kernel, %rax
	pushq	$0
	pushq	$__KERNEL_CS
	pushq	%rax
	lretq
	hlt

ENTRY(init_rsp)
	.quad  init_thread_union+THREAD_SIZE-8

.text
.code64

.balign PAGE_SIZE
#define NEXT_PAGE(name) \
	.balign	PAGE_SIZE; \
ENTRY(name)

#define PMDS(START, PERM, COUNT)		\
	i = 0 ;					\
	.rept (COUNT) ;				\
	.quad	(START) + (i << 21) + (PERM) ;	\
	i = i + 1 ;				\
	.endr

NEXT_PAGE(init_level4_pgt)
	.quad	level3_ident_pgt - __START_KERNEL_map + _KERNPG_TABLE
	.fill	257,8,0
	.quad	level3_ident_pgt - __START_KERNEL_map + _KERNPG_TABLE
	.fill	252,8,0
	/* (2^48-(2*1024*1024*1024))/(2^39) = 511 */
	.quad	level3_kernel_pgt - __START_KERNEL_map + _PAGE_TABLE

NEXT_PAGE(level3_ident_pgt)
	.quad	level2_ident_pgt - __START_KERNEL_map + _KERNPG_TABLE
	.fill	511,8,0

NEXT_PAGE(level3_kernel_pgt)
	.fill	510,8,0
	/* (2^48-(2*1024*1024*1024)-((2^39)*511))/(2^30) = 510 */
	.quad	level2_kernel_pgt - __START_KERNEL_map + _KERNPG_TABLE
	.quad	level2_fixmap_pgt - __START_KERNEL_map + _PAGE_TABLE

NEXT_PAGE(level2_fixmap_pgt)
	.fill	506,8,0
	.quad	level1_fixmap_pgt - __START_KERNEL_map + _PAGE_TABLE
	/* 8MB reserved for vsyscalls + a 2MB hole = 4 + 1 entries */
	.fill	5,8,0

NEXT_PAGE(level1_fixmap_pgt)
	.fill	512,8,0

NEXT_PAGE(level2_ident_pgt)
	/* Since I easily can, map the first 1G.
	 * Don't set NX because code runs from these pages.
	 */
	PMDS(0, __PAGE_KERNEL_LARGE_EXEC, PTRS_PER_PMD)

NEXT_PAGE(level2_kernel_pgt)
	/*
	 * 512 MB kernel mapping. We spend a full page on this pagetable
	 * anyway.
	 *
	 * The kernel code+data+bss must not be bigger than that.
	 *
	 * (NOTE: at +512MB starts the module area, see MODULES_VADDR.
	 *  If you want to increase this then increase MODULES_VADDR
	 *  too.)
	 */
	PMDS(0, __PAGE_KERNEL_LARGE_EXEC|_PAGE_GLOBAL, KERNEL_IMAGE_SIZE/PMD_SIZE)

NEXT_PAGE(level2_spare_pgt)
	.fill 512,8,0

#undef PMDS
#undef NEXT_PAGE

ENTRY(phys_base)
	/* This must match the first entry in level2_kernel_pgt */
	.quad   0x0000000000000000

	.section .data.head
	.align 16
head_cpu_gdt_descr:
	.word	gdt_end-cpu_gdt_table-1
	.quad	cpu_gdt_table - __START_KERNEL_map

    .data
	.align 16
	.globl cpu_gdt_descr
cpu_gdt_descr:
	.word	gdt_end-cpu_gdt_table-1
gdt:
	.quad	cpu_gdt_table

ENTRY(cpu_gdt_table)
	.quad	0x0000000000000000	/* NULL descriptor */
	.quad	0x00cf9b000000ffff	/* __KERNEL32_CS */
	.quad	0x00af9b000000ffff	/* __KERNEL_CS */
	.quad	0x00cf93000000ffff	/* __KERNEL_DS */
	.quad	0x00cffb000000ffff	/* __USER32_CS */
	.quad	0x00cff3000000ffff	/* __USER_DS, __USER32_DS  */
	.quad	0x00affb000000ffff	/* __USER_CS */
	.quad	0x0			/* unused */
	.quad	0,0			/* TSS */
	.quad	0,0			/* LDT */
	.quad   0,0,0			/* three TLS descriptors */ 
	.quad	0x0000f40000000000	/* node/CPU stored in limit */
gdt_end:

ENTRY(idt_table)
	.skip 256 * 16

	.section .bss.page_aligned, "aw", @nobits
	.align PAGE_SIZE
ENTRY(empty_zero_page)
	.skip PAGE_SIZE