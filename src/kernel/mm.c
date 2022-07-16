#include "mm.h"
#include "kprint.h"

extern char _start_kernel;
extern char _end_kernel;

uint_t init_msadrdsc(msadrdsc_t *msadrstart);

void init_memory() {
    // 初始化内存页
    gmdsc.msadrdscstart = (msadrdsc_t*)PAGE_4K_ALIGN(&_end_kernel);
    uint_t mdindx = init_msadrdsc(gmdsc.msadrdscstart);
    // uint_t 
    kprintf("msadrdsc start:%x size:%d\n", gmdsc.msadrdscstart, mdindx);

    // 初始化内存区

}

void msadrdsc_t_init(msadrdsc_t *initp)
{
	list_init(&initp->md_list);
    initp->md_flgs.mf_olkty = MF_OLKTY_INIT;
    initp->md_flgs.mf_lstty = MF_LSTTY_LIST;
    initp->md_flgs.mf_mocty = MF_MOCTY_FREE;
    initp->md_flgs.mf_marty = MF_MARTY_INIT;
    initp->md_flgs.mf_uindx = MF_UINDX_INIT;
    initp->md_phyadr.pf_alloc = PF_NO_ALLOC;
    initp->md_phyadr.pf_shared = PF_NO_SHARED;
    initp->md_phyadr.pf_swap = PF_NO_SWAP;
    initp->md_phyadr.pf_cache = PF_NO_CACHE;
    initp->md_phyadr.pf_kmap = PF_NO_KMAP;
    initp->md_phyadr.pf_lock = PF_NO_LOCK;
    initp->md_phyadr.pf_dirty = PF_NO_DIRTY;
    initp->md_phyadr.pf_busy = PF_NO_BUSY;
    initp->md_phyadr.pf_rv2 = PF_RV2_VAL;
    initp->md_phyadr.pf_phyadr = PF_INIT_PADRS;
	return;
}

void init_one_msadrdsc(msadrdsc_t *msadrs, uint_t phyadr) {
    msadrdsc_t_init(msadrs);

    phyadrflgs_t *tmp = (phyadrflgs_t*)&phyadr;
    msadrs->md_phyadr.pf_phyadr = tmp->pf_phyadr;
}

uint_t init_msadrdsc(msadrdsc_t *msadrstart) {
    uint_t mdindx = 0;
    for (uint32_t i = 0; i < gmdsc.e820_num; i++) {
        E820_t e820 = gmdsc.e820[i];
        if (e820.type != MEMORY_AVAILABLE) {
            continue;
        }

        uint_t end = e820.addr + e820.len;
        kinfo("available memorey area start address:%x end address:%x\n", e820.addr, end);
        for (uint_t start = e820.addr; start < end; start += PAGE_4K_SIZE) {
            // 剩下的空间是否够 4K
            if (start + PAGE_4K_SIZE - 1 < end) {
                init_one_msadrdsc(msadrstart+mdindx, start);
                mdindx++;
            }
        }
    }
    return mdindx;
}
