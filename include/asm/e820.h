#ifndef _ASM_E820_H
#define _ASM_E820_H

#include <os/ioport.h>

struct e820entry {
	__u64 addr;	/* start of memory segment */
	__u64 size;	/* size of memory segment */
	__u32 type;	/* type of memory segment */
} __attribute__((packed));



#endif