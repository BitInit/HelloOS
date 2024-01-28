#ifndef _ASM_X86_IO_H
#define _ASM_X86_IO_H

#include <asm/types.h>

static inline __u8 __readb(const volatile void *addr)
{
	return *(volatile __u8 *)addr;
}

static inline __u16 __readw(const volatile void *addr)
{
	return *(volatile __u16 *)addr;
}

static inline __u32 __readl(const volatile void *addr)
{
	return *(volatile __u32 *)addr;
}

static inline __u64 __readq(const volatile void *addr)
{
	return *(volatile __u64 *)addr;
}

#define readb(x) __readb(x)
#define readw(x) __readw(x)
#define readl(x) __readl(x)
#define readq(x) __readq(x)

static inline void __writel(__u32 b, volatile void *addr)
{
	*(volatile __u32 *)addr = b;
}

static inline void __writeq(__u64 b, volatile void *addr)
{
	*(volatile __u64 *)addr = b;
}

static inline void __writeb(__u8 b, volatile void *addr)
{
	*(volatile __u8 *)addr = b;
}

static inline void __writew(__u16 b, volatile void *addr)
{
	*(volatile __u16 *)addr = b;
}

#define writeq(val, addr) __writeq((val), (addr))
#define writel(val, addr) __writel((val), (addr))
#define writew(val, addr) __writew((val), (addr))
#define writeb(val, addr) __writeb((val), (addr))

#endif