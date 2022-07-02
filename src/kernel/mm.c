#include "mm.h"
#include "printk.h"

void init_memory() {
    uint64_t total_mem = 0;

    for (int32_t i = 0; i < global_mm_descriptor.e820_num; i++) {
        E820_t e820 = global_mm_descriptor.e820[i];
        if (e820.type != MEMORY_AVAILABLE) {
            continue;
        }
        
        total_mem += e820.len;
        printf("addr:%x len:%x type:%d\n", e820.addr, e820.len, e820.type);
    }
    printf("total mem:%d\n", total_mem);
}
