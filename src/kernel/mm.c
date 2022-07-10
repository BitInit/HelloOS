#include "mm.h"
#include "printk.h"

extern char _start_kernel;
extern char _end_kernel;

void print_global_mm() {
    kprintf("zone_addr:%x zone_num:%d zone_length:%d\n", global_mm_descriptor.zones, global_mm_descriptor.zone_num, global_mm_descriptor.zone_length);
    kprintf("bitmap_addr:%x bitmap_len:%d\n", global_mm_descriptor.bits_map, global_mm_descriptor.bits_length);
    kprintf("page_addr:%x pages_num:%d pages_length:%d \n", global_mm_descriptor.pages, global_mm_descriptor.pages_num, global_mm_descriptor.pages_length);
}

void init_memory() {
    for (int32_t i = 0; i < global_mm_descriptor.e820_num; i++) {
        E820_t e820 = global_mm_descriptor.e820[i];
        if (e820.type != MEMORY_AVAILABLE) {
            continue;
        }
        uint_t start_addr = PAGE_4K_ALIGN(e820.addr),
               end_addr = ((e820.addr + e820.len) >> PAGE_4K_SHIFT) << PAGE_4K_SHIFT;
        if (end_addr < start_addr) {
            continue;
        }
        global_mm_descriptor.zone_num++;
        global_mm_descriptor.pages_num += (end_addr - start_addr) >> PAGE_4K_SHIFT;
    }
    global_mm_descriptor.zone_length = global_mm_descriptor.zone_num * sizeof(mm_zone_t);
    global_mm_descriptor.zones = (mm_zone_t*)PAGE_4K_ALIGN(&_end_kernel);
    global_mm_descriptor.bits_map = (uint_t*)(global_mm_descriptor.zones + global_mm_descriptor.zone_num);
    global_mm_descriptor.bits_length = global_mm_descriptor.pages_num / 8 + 1;
    global_mm_descriptor.pages = (mm_page_t*)((uint8_t*)global_mm_descriptor.bits_map + global_mm_descriptor.bits_length);
    global_mm_descriptor.pages_length = global_mm_descriptor.pages_num * sizeof(mm_page_t);
    print_global_mm();

    // init zone
    mm_page_t *page = global_mm_descriptor.pages;
    mm_zone_t *tmp_zone = global_mm_descriptor.zones;
    for (int32_t i = 0; i < global_mm_descriptor.e820_num; i++) {
        E820_t e820 = global_mm_descriptor.e820[i];
        if (e820.type != MEMORY_AVAILABLE) {
            continue;
        }
        uint_t start_addr = PAGE_4K_ALIGN(e820.addr),
               end_addr = ((e820.addr + e820.len) >> PAGE_4K_SHIFT) << PAGE_4K_SHIFT;
        if (end_addr < start_addr) {
            continue;
        }

        tmp_zone->gmd = &global_mm_descriptor;
        tmp_zone->zone_start_addr = start_addr;
        tmp_zone->zone_end_addr = end_addr;
        tmp_zone->zone_length = end_addr - start_addr;
        tmp_zone->attribute = 0;
        tmp_zone->page_count = (end_addr - start_addr) >> PAGE_4K_SHIFT;
        tmp_zone->pages = page;
        
        tmp_zone++;
        page = page + tmp_zone->page_count;
        kprintf("addr:%x len:%x type:%d\n", e820.addr, e820.len, e820.type);
    }

    for (int32_t i = 0; i < global_mm_descriptor.zone_num; i++) {
        mm_zone_t zone = global_mm_descriptor.zones[i];
        kprintf("i:%d start_addr:%x end_addr:%x len:%d\n", i, zone.zone_start_addr, zone.zone_end_addr, zone.zone_length);
    }

    uint_t kernel_end_addr = (uint_t)global_mm_descriptor.pages + global_mm_descriptor.pages_length;
    kernel_end_addr = PAGE_4K_ALIGN(kernel_end_addr);
    kprintf("kernel end addr:%x\n", kernel_end_addr);
}
