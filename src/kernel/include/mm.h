#ifndef _KERNEL_MM_H
#define _KERNEL_MM_H

#include "type.h"

#define MEMORY_AVAILABLE              1
#define MEMORY_RESERVED               2
#define MEMORY_ACPI_RECLAIMABLE       3
#define MEMORY_NVS                    4
#define MEMORY_BADRAM                 5

typedef struct s_E820 {
    uint64_t    addr;
    uint64_t    len;
    uint32_t    type;
}__attribute__((packed)) E820_t;

typedef struct s_global_mem_descriptor {
    E820_t      e820[32];
    uint64_t    e820_num;
} global_mem_descriptor_t;

extern global_mem_descriptor_t global_mem_descriptor;

void init_memory();

#endif