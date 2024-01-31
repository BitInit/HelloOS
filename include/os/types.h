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

#ifndef _PTRDIFF_T
#define _PTRDIFF_T
typedef __kernel_ptrdiff_t	ptrdiff_t;
#endif

typedef		__u8		uint8_t;
typedef		__u16		uint16_t;
typedef		__u32		uint32_t;
typedef		__u64		uint64_t;

#endif