#include "mm.h"
#include "printk.h"

extern char _start_kernel;
extern char _end_kernel;

uint_t init_msadrdsc(msadrdsc_t *msadrstart);

void init_memory() {
    // 初始化内存页
    uint_t mdindx = 0;
    msadrdsc_t *msadrdscstart = (msadrdsc_t*)PAGE_4K_ALIGN(&_end_kernel);
    mdindx = init_msadrdsc(msadrdscstart);
    kprintf("msadrdsc start:%x size:%d\n", msadrdscstart, mdindx);

    // 初始化内存区
}

void init_one_msadrdsc(msadrdsc_t *msadrs, uint_t phyadr) {
    list_init(&msadrs->md_list);
    // msadrs->md_flgs.mf_olkty = MF_OLKTY_INIT;
    // msadrs->md_flgs.mf_lstty = MF_LSTTY_LIST;
    // msadrs->md_flgs.mf_mocty = MF_MOCTY_FREE;
    // msadrs->md_flgs.mf_marty = MF_MARTY_INIT;
    // msadrs->md_flgs.mf_uindx = MF_UINDX_INIT;

    // phyadrflgs_t *tmp = (phyadrflgs_t*)&phyadr;
    // msadrs->md_phyadr.pf_phyadr = tmp->pf_phyadr;
}

uint_t init_msadrdsc(msadrdsc_t *msadrstart) {
    uint_t mdindx = 0;
    for (uint32_t i = 0; i < gmdsc.e820_num; i++) {
        E820_t e820 = gmdsc.e820[i];
        if (e820.type != MEMORY_AVAILABLE) {
            continue;
        }

        uint_t end = e820.addr + e820.len;
        for (uint_t start = e820.addr; start < end; start += PAGE_4K_SIZE) {
            // 剩下的空间是否够 4K
            if (start + PAGE_4K_SIZE - 1 < end) {
                init_one_msadrdsc(&msadrstart[mdindx], start);
                mdindx++;
            }
        }
    }
    return mdindx;
}
