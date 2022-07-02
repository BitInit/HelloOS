#ifndef _KERNEL_MM_H
#define _KERNEL_MM_H

#include "type.h"

#define MEMORY_AVAILABLE              1
#define MEMORY_RESERVED               2
#define MEMORY_ACPI_RECLAIMABLE       3
#define MEMORY_NVS                    4
#define MEMORY_BADRAM                 5

typedef struct s_page mm_page_t;
typedef struct s_global_mm_descriptor global_mm_descriptor_t;

typedef struct {
    uint64_t    addr;
    uint64_t    len;
    uint32_t    type;
}__attribute__((packed)) E820_t;

typedef struct s_zone {
    mm_page_t   *page;
    uint64_t    page_count;
    uint64_t    zone_start_addr;    // 区起始地址
    uint64_t    zone_end_addr;      // 区结束地址
    uint64_t    zone_length;        // 区长度
    uint64_t    attribute;

    global_mm_descriptor_t *gmd;

    uint64_t    page_used_count;    // 区物理页已使用数
    uint64_t    page_free_count;    // 区物理页空闲数
    uint64_t    page_link_count;    // 区物理页被引用数
} mm_zone_t;

struct s_page {
    mm_zone_t   *zone;              // 所属区域
    uint64_t    phy_addr;           // 页物理地址
    uint64_t    attribute;          // 页属性
    uint64_t    ref_count;          // 页引用数
    uint64_t    ctime;              // 创建时间
};

struct s_global_mm_descriptor{
    E820_t      e820[32];
    uint64_t    e820_num;

    uint64_t    *bits_map;          // 位图地址
    uint64_t    bits_num;           // 总共物理页个数
    uint64_t    bits_length;        // 位图大小

    mm_zone_t   *zone;
    uint64_t    zone_num;
    uint64_t    zone_length;
};

extern global_mm_descriptor_t global_mm_descriptor;

void init_memory();

#endif