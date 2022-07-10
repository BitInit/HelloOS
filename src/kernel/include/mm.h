#ifndef _KERNEL_MM_H
#define _KERNEL_MM_H

#include "type.h"

#define MEMORY_AVAILABLE              1
#define MEMORY_RESERVED               2
#define MEMORY_ACPI_RECLAIMABLE       3
#define MEMORY_NVS                    4
#define MEMORY_BADRAM                 5

#define PAGE_4K_SHIFT                 12
#define PAGE_4K_SIZE                  (1UL << PAGE_4K_SHIFT)
#define PAGE_4K_MASK                  (~(PAGE_4K_SIZE - 1))
#define PAGE_4K_ALIGN(addr)           (((uint64_t)(addr) + PAGE_4K_SIZE - 1) & PAGE_4K_MASK)

typedef struct s_page mm_page_t;
typedef struct s_global_mm_descriptor global_mm_descriptor_t;

typedef struct {
    uint_t      addr;
    uint_t      len;
    uint32_t    type;
}__attribute__((packed)) E820_t;

typedef struct s_zone {
    mm_page_t   *pages;
    uint_t      page_count;
    uint_t      zone_start_addr;    // 区起始地址
    uint_t      zone_end_addr;      // 区结束地址
    uint_t      zone_length;        // 区长度
    uint_t      attribute;

    global_mm_descriptor_t *gmd;

    uint_t      page_used_count;    // 区物理页已使用数
    uint_t      page_free_count;    // 区物理页空闲数
    uint_t      page_link_count;    // 区物理页被引用数
} mm_zone_t;

struct s_page {
    mm_zone_t   *zone;              // 所属区域
    uint_t      phy_addr;           // 页物理地址
    uint_t      attribute;          // 页属性
    uint_t      ref_count;          // 页引用数
    uint_t      ctime;              // 创建时间
};

struct s_global_mm_descriptor{
    E820_t      e820[32];
    uint_t      e820_num;

    uint_t      *bits_map;          // 位图地址
    uint_t      bits_length;        // 位图大小

    mm_page_t   *pages;             // 页信息
    uint_t      pages_length;       // 页信息大小
    uint_t      pages_num;          // 页数

    mm_zone_t   *zones;
    uint_t      zone_num;
    uint_t      zone_length;
};

global_mm_descriptor_t global_mm_descriptor;

void init_memory();

#endif