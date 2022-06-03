#include "multiboot2.h"
#include "printk.h"
// #define VGB 0xb8000

int kernel_start(multiboot_uint32_t magic, multiboot_uint32_t addr)
{
    // unsigned int *p = (unsigned int*)VGB;
    // *p = 0x2f752f4d;
    clear_screen();
    printf("hello world: %x\n", magic);
    printf("addr %x\n", addr);
    while (1)
    {
        ;
    }
    
    // clear_screen();
    // printf("hello world");
    // if (magic != MULTIBOOT2_HEADER_MAGIC)
    // {
    //     return -1;
    // }
    
    
    // struct multiboot_tag *tag;
    // for (tag = (struct multiboot_tag*)(addr + MULTIBOOT_TAG_ALIGN);
    //      tag->type != MULTIBOOT_TAG_TYPE_END;
    //      tag = (struct multiboot_tag*)((multiboot_uint8_t *)tag + ((tag->size + 7) & ~7)))
    // {
        
    // }
    return 0;
}