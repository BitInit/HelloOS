#ifndef _OS_KERNEL_H
#define _OS_KERNEL_H

#include <stdarg.h>
#include <os/types.h>

#define	KERN_EMERG	"<0>"	/* system is unusable			*/
#define	KERN_ALERT	"<1>"	/* action must be taken immediately	*/
#define	KERN_CRIT	"<2>"	/* critical conditions			*/
#define	KERN_ERR	"<3>"	/* error conditions			*/
#define	KERN_WARNING	"<4>"	/* warning conditions			*/
#define	KERN_NOTICE	"<5>"	/* normal but significant condition	*/
#define	KERN_INFO	"<6>"	/* informational			*/
#define	KERN_DEBUG	"<7>"	/* debug-level messages			*/

#define	KERN_CONT	""

extern void __attribute__((format(printf, 1, 2)))
	early_printk(const char *fmt, ...);

extern int vscnprintf(char *buf, size_t size, const char *fmt, va_list args)
	__attribute__ ((format (printf, 3, 0)));

extern int vsnprintf(char *buf, size_t size, const char *fmt, va_list args)
	__attribute__ ((format (printf, 3, 0)));

// extern int printk(const char * fmt, ...)
// 	__attribute__((format (printf, 1, 2)));
#define printk(...) { \
	early_printk(__VA_ARGS__); \
}

#endif