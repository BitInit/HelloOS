#ifndef _OS_STRING_H
#define _OS_STRING_H

#include <asm/string.h>

#ifndef __HAVE_ARCH_MEMSET
extern void *memset(void *,int,__kernel_size_t);
#endif
#ifndef __HAVE_ARCH_STRNLEN
extern __kernel_size_t strnlen(const char *,__kernel_size_t);
#endif

#ifndef __HAVE_ARCH_MEMCPY
extern void* memcpy(void *dest, const void *src, size_t count);
#endif

#endif