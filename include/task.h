#ifndef _HELLOOS_TASK_H
#define _HELLOOS_TASK_H

#include "list.h"
#include "mm.h"
#include "gate.h"

#define CLONE_FS	(1 << 0)
#define CLONE_FILES	(1 << 1)
#define CLONE_SIGNAL	(1 << 2)

// 进程内存空间分布结构
struct mm_struct {
    pml4t_t *pgd;   // page table point

    unsigned long start_code, end_code;     // 代码段
    unsigned long start_data, end_data;     // 数据段
    unsigned long start_rodata, end_rodata; // 只读数据段
    unsigned long start_brk, end_brk;       // 动态内存区（堆）
    unsigned long start_stack;              // 进程栈空间
};

#define KERNEL_CS 	(0x08)
#define	KERNEL_DS 	(0x10)
#define	USER_CS		(0x28)
#define USER_DS		(0x30)

// 进程切换状态信息
struct thread_struct {
    unsigned long rsp0;         // 内核层栈基地址

    unsigned long rip;			// 内核层代码指针
    unsigned long rsp;			// 内核层当前栈指针

    unsigned long fs;
    unsigned long gs;

    unsigned long cr2;          // cr2 控制寄存器
    unsigned long trap_nr;      // 异常的异常号
    unsigned long error_code;   // 异常错误码
};

// 进程状态
#define TASK_RUNNING		(1 << 0)
#define TASK_INTERRUPTIBLE	(1 << 1)
#define	TASK_UNINTERRUPTIBLE	(1 << 2)
#define	TASK_ZOMBIE		(1 << 3)	
#define	TASK_STOPPED		(1 << 4)

#define PF_KTHREAD	(1 << 0)

struct task_struct {
    struct list_head list;
    volatile long state;
    unsigned long flags;

    struct mm_struct *mm;
    struct thread_struct *thread;

    unsigned long addr_limit;

    long pid;
    long counter;
    long signal;
    long priority;
};

#define STACK_SIZE 32768 // 32KB
union task_union {
    struct task_struct task;
    unsigned long stack[STACK_SIZE / sizeof(unsigned long)];
}__attribute__((aligned(8)));

struct mm_struct init_mm;
struct thread_struct init_thread;

#define INIT_TASK(tsk) {                \
    .state = TASK_INTERRUPTIBLE,        \
    .flags = PF_KTHREAD,                \
    .mm = &init_mm,                     \
    .thread = &init_thread,             \
    .addr_limit = 0xffff800000000000,   \
    .pid = 0,                           \
    .counter = 1,                       \
    .signal = 0,                        \
    .priority = 0                       \
}
union task_union init_task_union __attribute__((section(".data.init_task"))) = {INIT_TASK(init_task_union.task)};

#define NR_CPUS 2
struct task_struct *init_task[NR_CPUS] = {&init_task_union.task,0};

struct mm_struct init_mm = {0};

struct thread_struct init_thread = {
	.rsp0 = (unsigned long)(init_task_union.stack + STACK_SIZE / sizeof(unsigned long)),
	.rsp = (unsigned long)(init_task_union.stack + STACK_SIZE / sizeof(unsigned long)),
	.fs = KERNEL_DS,
	.gs = KERNEL_DS,
	.cr2 = 0,
	.trap_nr = 0,
	.error_code = 0
};

struct tss_struct {
	unsigned int  reserved0;
	unsigned long rsp0;
	unsigned long rsp1;
	unsigned long rsp2;
	unsigned long reserved1;
	unsigned long ist1;
	unsigned long ist2;
	unsigned long ist3;
	unsigned long ist4;
	unsigned long ist5;
	unsigned long ist6;
	unsigned long ist7;
	unsigned long reserved2;
	unsigned short reserved3;
	unsigned short iomapbaseaddr;
}__attribute__((packed));

#define INIT_TSS \
{	.reserved0 = 0,	 \
	.rsp0 = (unsigned long)(init_task_union.stack + STACK_SIZE / sizeof(unsigned long)),	\
	.rsp1 = (unsigned long)(init_task_union.stack + STACK_SIZE / sizeof(unsigned long)),	\
	.rsp2 = (unsigned long)(init_task_union.stack + STACK_SIZE / sizeof(unsigned long)),	\
	.reserved1 = 0,	 \
	.ist1 = 0xffff800000007c00,	\
	.ist2 = 0xffff800000007c00,	\
	.ist3 = 0xffff800000007c00,	\
	.ist4 = 0xffff800000007c00,	\
	.ist5 = 0xffff800000007c00,	\
	.ist6 = 0xffff800000007c00,	\
	.ist7 = 0xffff800000007c00,	\
	.reserved2 = 0,	\
	.reserved3 = 0,	\
	.iomapbaseaddr = 0	\
}
struct tss_struct init_tss[NR_CPUS] = { [0 ... NR_CPUS-1] = INIT_TSS };

static inline struct task_struct* get_current() {
	struct task_struct *current = NULL;
	__asm__ __volatile__ ("andq %%rsp, %0	\n\t":"=r"(current):"0"(~32767UL));
	return current;
}

#define current get_current()

#define GET_CURRENT			\
	"movq	%rsp,	%rbx	\n\t"	\
	"andq	$-32768,%rbx	\n\t"

#define switch_to(prev, next)	\
do {							\
	__asm__ __volatile__ (	"pushq	%%rbp	\n\t"	\
				"pushq	%%rax	\n\t"	\
				"movq	%%rsp,	%0	\n\t"	\
				"movq	%2,	%%rsp	\n\t"	\
				"leaq	1f(%%rip),	%%rax	\n\t"	\
				"movq	%%rax,	%1	\n\t"	\
				"pushq	%3		\n\t"	\
				"jmp	__switch_to	\n\t"	\
				"1:	\n\t"	\
				"popq	%%rax	\n\t"	\
				"popq	%%rbp	\n\t"	\
				:"=m"(prev->thread->rsp),	\
				 "=m"(prev->thread->rip)	\
				:"m"(next->thread->rsp),	\
				 "m"(next->thread->rip),	\
				 "D"(prev),	\
				 "S"(next)	\
				:"memory"	\
				);			\
} while(0)

void task_init();

#endif