#ifndef _HELLOOS_MM_H
#define _HELLOOS_MM_H

#include "types.h"

#define KERNEL_VIRADDR                0xffff800000000000
#define PAGE_4K_SHIFT                 12
#define PAGE_4K_SIZE                  (1UL << PAGE_4K_SHIFT)
#define PAGE_4K_MASK                  (~(PAGE_4K_SIZE - 1))
#define PAGE_4K_ALIGN(addr)           (((uint_t)(addr) + PAGE_4K_SIZE - 1) & PAGE_4K_MASK)
#define PHY2VIR(addr)                 (KERNEL_VIRADDR | ((uint_t)(addr)))
#define VIR2PHY(addr)                 ((~KERNEL_VIRADDR) & ((uint_t)(addr)))

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

//struct page attribute (alloc_pages flags)
#define PG_PTable_Maped	(1 << 0)
#define PG_Kernel_Init	(1 << 1)
#define PG_Referenced	(1 << 2)
#define PG_Dirty	(1 << 3)
#define PG_Active	(1 << 4)
#define PG_Up_To_Date	(1 << 5)
#define PG_Device	(1 << 6)
#define PG_Kernel	(1 << 7)
#define PG_K_Share_To_U	(1 << 8)
#define PG_Slab		(1 << 9)
typedef struct {
    uint_t      phy_addr;
    uint64_t    attribute;
    uint_t      ref_count;
    uint_t      age;
    mm_zone_t   *zone;
} mm_page_t;

#define ZONE_DMA	(1 << 0)
#define ZONE_NORMAL	(1 << 1)
#define ZONE_UNMAPED	(1 << 2)

int ZONE_DMA_INDEX	= 0;
int ZONE_NORMAL_INDEX	= 0;
int ZONE_UNMAPED_INDEX	= 0;

struct _mm_zone {
    mm_page_t   *pages_group;
    uint_t      pages_length;

    uint_t      zone_start_addr;
    uint_t      zone_end_addr;
    uint_t      zone_length;
    uint64_t    attribute;

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

unsigned long page_init(mm_page_t *page, uint_t flags);

mm_page_t* alloc_pages(int zone_select, int n, uint64_t flags);

static inline uint_t* get_gdt() {
    uint_t *tmp;
	__asm__ __volatile__(
        "movq	%%cr3,	%0	\n\t"
        :"=r"(tmp)
        :
        :"memory"
	);
	return tmp;
}

#endif