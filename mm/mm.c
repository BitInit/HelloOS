#include "mm.h"
#include "kprint.h"
#include "kstring.h"

extern uint_t _start_kernel;
extern uint_t _end_kernel;

void page_bitmap_init(uint_t start_addr);
void pages_struct_init(uint_t start_addr);
void zones_struct_init(uint_t start_addr);
void construct_mem_struct();

void init_memory() {
    gmdsc.kernel_start_phyaddr = (uint_t)&_start_kernel;
    gmdsc.kernel_end_phyaddr = (uint_t)&_end_kernel;
    kinfo("kernel code start_phyaddr:%x end_phyaddr:%x\n", gmdsc.kernel_start_phyaddr, gmdsc.kernel_end_phyaddr);

    uint_t start_addr = PAGE_4K_ALIGN(gmdsc.kernel_end_phyaddr);
    page_bitmap_init(start_addr);

    start_addr = PAGE_4K_ALIGN(gmdsc.page_bits + gmdsc.page_bits_length);
    pages_struct_init(start_addr);

    start_addr = PAGE_4K_ALIGN(gmdsc.pages_struct + gmdsc.pages_size);
    zones_struct_init(start_addr);

    construct_mem_struct();
}

void page_bitmap_init(uint_t start_addr) {
    E820_t *e820 = &gmdsc.e820[gmdsc.e820_num-1];
    uint_t total_mem = e820->addr + e820->len;
    kdebug("total memory: %d\n", total_mem);

    gmdsc.total_mem_length = total_mem;
    gmdsc.page_bits = (uint_t*)start_addr;
    gmdsc.page_bits_size = total_mem >> PAGE_4K_SHIFT;
    gmdsc.page_bits_length = (((uint_t)(total_mem >> PAGE_4K_SHIFT) + sizeof(uint_t) * 8 - 1) / 8) & ( ~ (sizeof(uint_t) - 1));
    kinfo("page_bitmap start_addr:%x page_bits_size:%d page_bits_length:%d\n", gmdsc.page_bits, gmdsc.page_bits_size, gmdsc.page_bits_length);
    memset(gmdsc.page_bits, 0xff, gmdsc.page_bits_length);
}

void pages_struct_init(uint_t start_addr) {
    gmdsc.pages_struct = (mm_page_t*)start_addr;
    gmdsc.pages_size = gmdsc.total_mem_length >> PAGE_4K_SHIFT;
    gmdsc.pages_length = (gmdsc.pages_size * sizeof(mm_page_t) + sizeof(uint_t) - 1) & ( ~ (sizeof(uint_t) - 1));
    kinfo("pages_struct start_addr:%x pages_size:%d pages_length:%d\n", gmdsc.pages_struct, gmdsc.pages_size, gmdsc.pages_length);
    memset(gmdsc.pages_struct, 0x00, gmdsc.pages_length);
}

void zones_struct_init(uint_t start_addr) {
    gmdsc.zones_struct = (mm_zone_t*)start_addr;
    gmdsc.zones_size = gmdsc.e820_available_size;
    gmdsc.zones_length = (gmdsc.zones_size * sizeof(mm_zone_t) + sizeof(uint_t) - 1) & (~(sizeof(uint_t) - 1));
    kinfo("zones_struct start_addr:%x zones_size:%d zones_length:%d\n", gmdsc.zones_struct, gmdsc.zones_size, gmdsc.zones_length);
    memset(gmdsc.zones_struct, 0x00, gmdsc.zones_length);
}

void construct_mem_struct() {
    uint32_t zone_idx = 0;
    for (uint32_t i = 0; i < gmdsc.e820_num; i++) {
        uint_t start, end;
        E820_t *e820 = &gmdsc.e820[i];
        if (e820->type != E820_TYPE_AVAILABLE) continue;
        start = PAGE_4K_ALIGN(e820->addr);
        end = ((e820->addr + e820->len) >> PAGE_4K_SHIFT) << PAGE_4K_SHIFT;
        if (end <= start) continue;

        mm_zone_t *z = gmdsc.zones_struct + zone_idx;
        z->zone_start_addr = start;
        z->zone_end_addr = end;
        z->zone_length = end - start;
        z->page_using_count = 0;
        z->page_free_count = (end - start) >> PAGE_4K_SHIFT;
        z->total_pages_link = 0;
        z->attribute = 0;
        z->pages_length = (end - start) >> PAGE_4K_SHIFT;
        z->pages_group = (mm_page_t*)(gmdsc.pages_struct + (start >> PAGE_4K_SHIFT));

        mm_page_t *p = z->pages_group;
        for (uint32_t j = 0; j < z->pages_length; j++, p++) {
            p->zone = z;
            p->phy_addr = start + PAGE_4K_SIZE*j;
            p->attribute = 0;
            p->ref_count = 0;
            p->age = 0;
            *(gmdsc.page_bits + ((p->phy_addr >> PAGE_4K_SHIFT) >> 6)) ^= 1UL << (p->phy_addr >> PAGE_4K_SHIFT) % 64;
        }
        zone_idx++;
    }
}