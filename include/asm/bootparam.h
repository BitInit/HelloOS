#ifndef _ASM_X86_BOOTPARAM_H
#define _ASM_X86_BOOTPARAM_H

#include <asm/e820.h>

struct boot_params {
    __u32  e820_entries;
    struct e820entry e820_map[E820MAX];
};

extern struct boot_params boot_params;

#endif