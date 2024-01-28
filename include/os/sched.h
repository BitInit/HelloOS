#ifndef _OS_SCHED_H
#define _OS_SCHED_H

#include <asm/thread_info.h>
#include <asm/page.h>

union thread_union {
	struct thread_info thread_info;
	unsigned long stack[THREAD_SIZE/sizeof(unsigned long)];
};

extern union thread_union init_thread_union;

#endif