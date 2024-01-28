// 系统启动时，物理内存分配器
#ifndef _OS_BOOTMEM_H
#define _OS_BOOTMEM_H

#include <mmzone.h>

// 低端内存的最大页帧号
extern unsigned long max_low_pfn;
// 低端内存的最小页帧号
extern unsigned long min_low_pfn;

// 最大内存页
extern unsigned long max_pfn;

typedef struct bootmem_data {
    unsigned long node_boot_start; // 节点的起始地址，UNMA 中会有多个节点，但我们只支持 NUM，该值为 0
    unsigned long node_low_pfn; // 低端内存的最后一个页帧号

    void *node_bootmem_map; // 内存 bitmap 地址


} bootmem_data_t;

#endif