#include "multiboot2.h"
#include "printk.h"
#include "mm.h"

extern global_mm_descriptor_t gmdsc;
static void parse_sys_info(multiboot_uint64_t mb2_info_addr);

int kernel_start(multiboot_uint64_t mb2_magic, multiboot_uint64_t mb2_info_addr) {
    if (mb2_magic != MULTIBOOT2_BOOTLOADER_MAGIC || mb2_info_addr & 7) {
        return -1;
    }
    // 清屏
    clear_screen();
    // 解析系统信息
    parse_sys_info(mb2_info_addr);

    while (1) ;
    return 0;
}

static void _init_memory(struct multiboot_tag *tag);

static void parse_sys_info(multiboot_uint64_t mb2_info_addr) {
    struct multiboot_tag *tag;
    for (tag = (struct multiboot_tag*)(mb2_info_addr + MULTIBOOT_TAG_ALIGN);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (struct multiboot_tag*)((multiboot_uint8_t *)tag + ((tag->size + 7) & ~7))) {
        switch (tag->type) {
        case MULTIBOOT_TAG_TYPE_MMAP:
            _init_memory(tag);
            break;
        default:
            break;
        }
    }
}

static void _init_memory(struct multiboot_tag *tag) {
    struct multiboot_tag_mmap *mmap = (struct multiboot_tag_mmap*) tag;
    struct multiboot_mmap_entry *entry = mmap->entries;

    uint32_t i = 0;
    while ((uint8_t*)entry < (uint8_t*)mmap + mmap->size) {
        gmdsc.e820[i].addr = entry->addr;
        gmdsc.e820[i].len = entry->len;
        gmdsc.e820[i].type = entry->type;
        i++;
        
        entry = (struct multiboot_mmap_entry*)((uint8_t*)entry + mmap->entry_size);
    }
    gmdsc.e820_num = i;
    init_memory();
}