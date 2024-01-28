#ifndef _OS_TYPES_H
#define _OS_TYPES_H

#include <asm/types.h>

#ifndef _SIZE_T
#define _SIZE_T
typedef __kernel_size_t	size_t;
#endif

#ifndef _SSIZE_T
#define _SSIZE_T
typedef __kernel_ssize_t	ssize_t;
#endif

#endif