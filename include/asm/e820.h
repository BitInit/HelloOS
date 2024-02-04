#ifndef _ASM_E820_H
#define _ASM_E820_H

#include <os/ioport.h>

#define E820MAX	128

#define E820_RAM	1
#define E820_RESERVED	2
#define E820_ACPI	3
#define E820_NVS	4

struct e820entry {
	__u64 addr;	/* start of memory segment */
	__u64 size;	/* size of memory segment */
	__u32 type;	/* type of memory segment */
} __attribute__((packed));

struct e820map {
	__u32 nr_map;
	struct e820entry map[E820MAX];
};

extern struct e820map e820;
#endif