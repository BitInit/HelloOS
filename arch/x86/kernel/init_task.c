#include <os/sched.h>
#include <asm/thread_info.h>

union thread_union init_thread_union
	__attribute__((__section__(".data.init_task"))) =
		{ INIT_THREAD_INFO(init_task) };


