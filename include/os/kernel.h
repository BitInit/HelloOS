#ifndef _OS_KERNEL_H
#define _OS_KERNEL_H

#include <stdarg.h>
#include <os/types.h>

extern void __attribute__((format(printf, 1, 2)))
	early_printk(const char *fmt, ...);

extern int vscnprintf(char *buf, size_t size, const char *fmt, va_list args)
	__attribute__ ((format (printf, 3, 0)));

extern int vsnprintf(char *buf, size_t size, const char *fmt, va_list args)
	__attribute__ ((format (printf, 3, 0)));

#endif