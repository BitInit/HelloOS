#include <asm-generic/sections.h>

#include <os/string.h>
#include <os/start_kernel.h>
#include <os/kernel.h>

#include <asm/bootparam.h>
#include <asm/multiboot2.h>

extern uint64_t mb2_info;

void clear_bss() {
    memset(__bss_start, 0,
	       (unsigned long) __bss_stop - (unsigned long) __bss_start);
}

static void mem_info(struct multiboot_tag *tag) {
    struct multiboot_tag_mmap *mmap = (struct multiboot_tag_mmap*) tag;
    struct multiboot_mmap_entry *entry = mmap->entries;

    uint32_t i = 0;
    while ((uint8_t*)entry < (uint8_t*)mmap + mmap->size) {
        boot_params.e820_map[i].addr = entry->addr;
        boot_params.e820_map[i].size = entry->len;
        boot_params.e820_map[i].type = entry->type;

        i++;
        entry = (struct multiboot_mmap_entry*)((uint8_t*)entry + mmap->entry_size);
    }
    boot_params.e820_entries = i;
}

void parse_sys_info() {
    struct multiboot_tag *tag;
    for (tag = (struct multiboot_tag*)(mb2_info + MULTIBOOT_TAG_ALIGN);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (struct multiboot_tag*)((multiboot_uint8_t *)tag + ((tag->size + 7) & ~7))) {
        switch (tag->type) {
        case MULTIBOOT_TAG_TYPE_MMAP:
            mem_info(tag);
            break;
        default:
            break;
        }
    }
}

void x86_64_start_kernel() {
    early_printk("x86_64_start_kernel running\n");
    // bss 段清零
    clear_bss();
    // 解析系统信息
    parse_sys_info();

    start_kernel();
}