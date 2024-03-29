#include "task.h"
#include "ptrace.h"
#include "kprint.h"
#include "lib.h"
#include "kstring.h"

extern void ret_from_intr();
extern void system_call(void);
extern void ret_system_call(void);

void __switch_to(struct task_struct *prev,struct task_struct *next) {

	init_tss[0].rsp0 = next->thread->rsp0;

	set_tss64(init_tss[0].rsp0, init_tss[0].rsp1, init_tss[0].rsp2, init_tss[0].ist1, init_tss[0].ist2, init_tss[0].ist3, init_tss[0].ist4, init_tss[0].ist5, init_tss[0].ist6, init_tss[0].ist7);

	__asm__ __volatile__("movq	%%fs,	%0 \n\t":"=a"(prev->thread->fs));
	__asm__ __volatile__("movq	%%gs,	%0 \n\t":"=a"(prev->thread->gs));
	kinfo("next:%lx\n", next);

	__asm__ __volatile__("movq	%0,	%%fs \n\t"::"a"(next->thread->fs));
	__asm__ __volatile__("movq	%0,	%%gs \n\t"::"a"(next->thread->gs));

	kinfo("prev->thread->rsp0:%#018lx\n",prev->thread->rsp0);
	kinfo("next->thread->rsp0:%#018lx\n",next->thread->rsp0);
}

void user_level_function() {
	long ret = 0;
	kinfo("user_level_function task is running\n");
	char str[] = "Hello World! \n";

	__asm__	__volatile__(	"leaq	sysexit_return_address(%%rip),	%%rdx	\n\t"
		"movq	%%rsp,	%%rcx		\n\t"
		"sysenter			\n\t"
		"sysexit_return_address:	\n\t"
		:"=a"(ret):"0"(1), "D"(str):"memory");

	kinfo("user_level_function task called sysenter,ret:%ld\n",ret);

	while(1);
}

unsigned long do_execve(struct pt_regs * regs) {
	regs->rdx = 0x800000;	//RIP
	regs->rcx = 0xa00000;	//RSP
	regs->rax = 1;
	regs->ds = 0;
	regs->es = 0;
	kinfo("do_execve task is running\n");

	memcpy(user_level_function,(void *)0x800000,1024);

	return 0;
}

unsigned long do_exit(unsigned long code) {
	kinfo("exit task is running,arg:%#018lx\n", code);
	while(1);
}

unsigned long init(unsigned long arg) {
	struct pt_regs *regs;

	kinfo("init task is running,arg:%#018lx\n",arg);

	current->thread->rip = (unsigned long)ret_system_call;
	current->thread->rsp = (unsigned long)current + STACK_SIZE - sizeof(struct pt_regs);
	regs = (struct pt_regs *)current->thread->rsp;

	__asm__	__volatile__("movq	%1,	%%rsp	\n\t"
		"pushq  %2	\n\t"
		"jmp	do_execve	\n\t"
		::"D"(regs),"m"(current->thread->rsp),"m"(current->thread->rip):"memory");

	return 1;
}

extern void kernel_thread_func(void);
__asm__ (
"kernel_thread_func:	\n\t"
"	popq	%r15	\n\t"
"	popq	%r14	\n\t"	
"	popq	%r13	\n\t"	
"	popq	%r12	\n\t"	
"	popq	%r11	\n\t"	
"	popq	%r10	\n\t"	
"	popq	%r9	\n\t"	
"	popq	%r8	\n\t"	
"	popq	%rbx	\n\t"	
"	popq	%rcx	\n\t"	
"	popq	%rdx	\n\t"	
"	popq	%rsi	\n\t"	
"	popq	%rdi	\n\t"	
"	popq	%rbp	\n\t"	
"	popq	%rax	\n\t"	
"	movq	%rax,	%ds	\n\t"
"	popq	%rax		\n\t"
"	movq	%rax,	%es	\n\t"
"	popq	%rax		\n\t"
"	addq	$0x38,	%rsp	\n\t"
/////////////////////////////////
"	movq	%rdx,	%rdi	\n\t"
"	callq	*%rbx		\n\t"
"	movq	%rax,	%rdi	\n\t"
"	callq	do_exit		\n\t"
);

unsigned long do_fork(struct pt_regs * regs, unsigned long clone_flags, unsigned long stack_start, unsigned long stack_size) {
	struct task_struct *tsk = NULL;
	struct thread_struct *thd = NULL;
	mm_page_t *p = alloc_pages(ZONE_NORMAL, 8, PG_PTable_Maped | PG_Active | PG_Kernel);

	tsk = (struct task_struct*)PHY2VIR(p->phy_addr);
	kinfo("struct task_struct address:%#018lx\n", (unsigned long)tsk);

	memset(tsk, 0, sizeof(struct task_struct)+sizeof(struct thread_struct));
	*tsk = *current;

	list_init(&tsk->list);
	list_add_tail(&init_task_union.task.list, &tsk->list);	
	tsk->pid++;
	tsk->state = TASK_UNINTERRUPTIBLE;

	thd = (struct thread_struct*)(tsk + 1);
	tsk->thread = thd;

	memcpy(regs, (void *)((unsigned long)tsk + STACK_SIZE - sizeof(struct pt_regs)), sizeof(struct pt_regs));

	thd->rsp0 = (unsigned long)tsk + STACK_SIZE;
	thd->rip = regs->rip;
	thd->rsp = (unsigned long)tsk + STACK_SIZE - sizeof(struct pt_regs);

	if(!(tsk->flags & PF_KTHREAD))
		thd->rip = regs->rip = (unsigned long)ret_system_call;
	
	tsk->state = TASK_RUNNING;
	return 0;
}

int kernel_thread(unsigned long (* fn)(unsigned long), unsigned long arg, unsigned long flags) {
	struct pt_regs regs;
	memset(&regs, 0, sizeof(regs));

	regs.rbx = (unsigned long)fn;
	regs.rdx = (unsigned long)arg;

	regs.ds = KERNEL_DS;
	regs.es = KERNEL_DS;
	regs.cs = KERNEL_CS;
	regs.ss = KERNEL_DS;
	regs.rflags = (1 << 9);
	regs.rip = (unsigned long)kernel_thread_func;

	return do_fork(&regs, flags, 0, 0);
}

void task_init() {
	init_mm.pgd = (pml4t_t*) Global_CR3;
	init_mm.start_code = gmdsc.start_text;
	init_mm.end_code = gmdsc.end_text;
	init_mm.start_data = gmdsc.start_data;
	init_mm.end_data = gmdsc.end_data;
	init_mm.start_rodata = gmdsc.start_rodata;
	init_mm.end_rodata = gmdsc.end_rodata;
	init_mm.start_brk = 0;
	init_mm.end_brk = (unsigned long)PHY2VIR(gmdsc.kernel_end_phyaddr);
	init_mm.start_stack = gmdsc.start_stack;

	wrmsr(0x174, KERNEL_CS);
	wrmsr(0x175, current->thread->rsp0);
	wrmsr(0x176, (unsigned long) system_call);

	set_tss64(init_tss[0].rsp0, init_tss[0].rsp1, init_tss[0].rsp2, init_tss[0].ist1, init_tss[0].ist2, init_tss[0].ist3, init_tss[0].ist4, init_tss[0].ist5, init_tss[0].ist6, init_tss[0].ist7);
	init_tss[0].rsp0 = init_thread.rsp0;

	list_init(&init_task_union.task.list);

	kernel_thread(init, 10, CLONE_FS|CLONE_FILES|CLONE_SIGNAL);

	init_task_union.task.state = TASK_RUNNING;
	struct task_struct *p = container_of(list_next(&current->list), struct task_struct, list);
	switch_to(current, p);
}