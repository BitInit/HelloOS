#ifndef _ASM_X86_PAGE_H
#define _ASM_X86_PAGE_H

#include <os/const.h>

/*
 * 页面大小等
 */
#define PAGE_SHIFT	12
#define PAGE_SIZE	(_AC(1,UL) << PAGE_SHIFT)
#define PAGE_MASK	(~(PAGE_SIZE-1))
#define PAGE_OFFSET _AC(0xffff810000000000, UL)

#define THREAD_ORDER	1
#define THREAD_SIZE  (PAGE_SIZE << THREAD_ORDER)

#define PMD_SHIFT	21
#define PTRS_PER_PMD	512
#define PMD_SIZE	(_AC(1, UL) << PMD_SHIFT)
#define PMD_MASK	(~(PMD_SIZE - 1))

#define __PHYSICAL_START	0x200000
#define __START_KERNEL_map	_AC(0xffffffff80000000, UL)
#define __START_KERNEL		(__START_KERNEL_map + __PHYSICAL_START)

/*
 * 内核镜像限制为 512MB
 */
#define KERNEL_IMAGE_SIZE	(512 * 1024 * 1024)
#define KERNEL_IMAGE_START	_AC(0xffffffff80000000, UL)

#define __pa(x)		((unsigned long)(x)-PAGE_OFFSET)
#define __va(x)			((void *)((unsigned long)(x)+PAGE_OFFSET))


#endif