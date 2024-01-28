#ifndef _OS_STRING_H
#define _OS_STRING_H

#include <asm/string.h>

#ifndef __HAVE_ARCH_MEMSET
extern void *memset(void *,int,__kernel_size_t);
#endif

#endif