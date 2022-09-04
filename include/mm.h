#ifndef _HELLOOS_MM_H
#define _HELLOOS_MM_H

#include "types.h"

#define KERNEL_VIRADDR                0xffff800000000000
#define PAGE_4K_SHIFT                 12
#define PAGE_4K_SIZE                  (1UL << PAGE_4K_SHIFT)
#define PAGE_4K_MASK                  (~(PAGE_4K_SIZE - 1))
#define PAGE_4K_ALIGN(addr)           (((uint_t)(addr) + PAGE_4K_SIZE - 1) & PAGE_4K_MASK)
#define PHY2VIR(addr)                 (KERNEL_VIRADDR + addr)
#define VIR2PHY(addr)                 (addr - KERNEL_VIRADDR)

#define E820_TYPE_AVAILABLE              1
#define E820_TYPE_RESERVED               2
#define E820_TYPE_ACPI_RECLAIMABLE       3
#define E820_TYPE_NVS                    4
#define E820_TYPE_BADRAM                 5
typedef struct {
    uint_t      addr;
    uint_t      len;
    uint32_t    type;
}__attribute__((packed)) E820_t;

typedef struct _mm_zone mm_zone_t;

typedef struct {
    uint_t      phy_addr;
    uint_t      attribute;
    uint_t      ref_count;
    uint_t      age;
    mm_zone_t   *zone;
} mm_page_t;

struct _mm_zone {
    mm_page_t   *pages_group;
    uint_t      pages_length;

    uint_t      zone_start_addr;
    uint_t      zone_end_addr;
    uint_t      zone_length;
    uint_t      attribute;

    uint_t      page_using_count;
    uint_t      page_free_count;
    uint_t      total_pages_link;
};

typedef struct {
    E820_t      e820[32];
    uint_t      e820_num;
    uint_t      e820_available_size;

    uint_t      *page_bits;
    uint_t      page_bits_size;
    uint_t      page_bits_length;

    mm_page_t   *pages_struct;
    uint_t      pages_size;
    uint_t      pages_length;

    mm_zone_t   *zones_struct;
    uint_t      zones_size;
    uint_t      zones_length;

    uint_t      total_mem_length;       // 总内存大小
    uint_t      kernel_start_phyaddr;   // 内核程序开始物理地址
    uint_t      kernel_end_phyaddr;     // 内核程序结束物理地址
    uint_t      end_of_struct;          // 内存管理结构的结束地址
} global_mm_descriptor_t;

global_mm_descriptor_t gmdsc;

void init_memory();

#endif