#include "mm.h"
#include "kprint.h"

extern char _start_kernel;
extern char _end_kernel;

uint_t init_msadrdsc(msadrdsc_t *msadrstart);
void init_memarea();
bool_t init_memarea_data();

void init_memory() {
    gmdsc.kernel_start_addr = (uint_t)&_start_kernel;
    gmdsc.kernel_end_addr = (uint_t)&_end_kernel;
    kinfo("kernel code start_address:%x end_address:%x\n", gmdsc.kernel_start_addr, gmdsc.kernel_end_addr);

    // 初始化内存页
    gmdsc.msadrdscstart = (msadrdsc_t*)PAGE_4K_ALIGN(gmdsc.kernel_end_addr);
    uint_t mdindx = init_msadrdsc(gmdsc.msadrdscstart);
    gmdsc.msadrdsc_sz = mdindx;
    kinfo("msadrdsc_t start_address:%x page number:%d\n", gmdsc.msadrdscstart, mdindx);
    gmdsc.kernel_end_addr = (uint_t)(gmdsc.msadrdscstart + gmdsc.msadrdsc_sz);
    kinfo("kernel_end_addr:%x\n", gmdsc.kernel_end_addr);

    // 初始化内存区
    init_memarea();

    // 内核占用的内存页设置成已占用
    init_memarea_data();

    msadrdsc_t *ms = alloc_page(MA_TYPE_KRNL);
    kerror("ms:%x addr:%x\n", ms, ms->md_phyadr.pf_phyadr << PAGE_4K_SHIFT);
}

void msadrdsc_t_init(msadrdsc_t *initp) {
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
        kinfo("available memorey area start_address:%x end_address:%x\n", e820.addr, end);
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

void _init_memarea(memarea_t *ma) {
    ma->ma_stus = 0;
    ma->ma_flgs = 0;
    ma->ma_horizline = 0;

    ma->bafhlst.af_objnr = 0;
    ma->bafhlst.af_freeobjnr = 0;
    ma->bafhlst.af_allocindx = 0;
    ma->bafhlst.af_freeindx = 0;
    ma->bafhlst.af_resvpg = 0;
    list_init(&ma->bafhlst.af_freelst);
    list_init(&ma->bafhlst.af_alloclst); 
}

void init_memarea() {
    // 硬件区初始化
    memarea_t *ma = &gmdsc.memarea[0];
    ma->ma_type = MA_TYPE_HWAD;
    ma->ma_startadr = MA_HWAD_START;
    ma->ma_endadr = MA_HWAD_END;
    ma->ma_sz = MA_HWAD_LEN;
    _init_memarea(ma);
    
    // 内核区初始化
    ma = &gmdsc.memarea[1];
    ma->ma_type = MA_TYPE_KRNL;
    ma->ma_startadr = MA_KRNL_START;
    ma->ma_endadr = MA_KRNL_END;
    ma->ma_sz = MA_KRNL_LEN;
    _init_memarea(ma);

    // 进程区初始化
    ma = &gmdsc.memarea[2];
    ma->ma_type = MA_TYPE_PROC;
    ma->ma_startadr = MA_PROC_START;
    ma->ma_endadr = MA_PROC_END;
    ma->ma_sz = MA_PROC_LEN;
    _init_memarea(ma);
}

uint_t search_occupymsadrdsc(memarea_t *ma, uint_t ocpystart, uint_t ocpyend) {
    msadrdsc_t *msadrdscstart = gmdsc.msadrdscstart, *target = NULL;
    uint_t msadrdsc_sz = gmdsc.msadrdsc_sz;
    int i = 0;
    for (uint_t mnr = 0; mnr < msadrdsc_sz; mnr++) {
        if (i < 180) {
            // kdebug("mnr:%d addr:%x\n", mnr, msadrdscstart[mnr].md_phyadr.pf_phyadr);
            i++;
        }
        if ((msadrdscstart[mnr].md_phyadr.pf_phyadr << PAGE_4K_SHIFT) == ocpystart) {
            target = msadrdscstart + mnr;
            break;
        }
    }
    if (target == NULL) {
        return 0;
    }
    uint_t mnr = 0;
    for (uint_t tmpaddr = ocpystart; tmpaddr < ocpyend; tmpaddr += PAGE_4K_SIZE, mnr++) {
        uint_t phyaddr = target[mnr].md_phyadr.pf_phyadr << PAGE_4K_SHIFT;
        if (tmpaddr != phyaddr) {
            return 0;
        }
        target[mnr].md_flgs.mf_mocty = MF_MOCTY_KRNL;
        target[mnr].md_flgs.mf_uindx++;
        target[mnr].md_phyadr.pf_alloc = PF_ALLOC;
        ma->bafhlst.af_resvpg++;
    }
    return mnr;
}

bool_t build_bafhlst(memarea_t *ma) {
    uint_t startadr = ma->ma_startadr, endadr = ma->ma_endadr;
    msadrdsc_t *msadrdscstart = gmdsc.msadrdscstart, *target = NULL;
    uint_t mnr = 0;
    for (; mnr < gmdsc.msadrdsc_sz; mnr++) {
        if ((msadrdscstart[mnr].md_phyadr.pf_phyadr << PAGE_4K_SHIFT) == startadr) {
            target = msadrdscstart + mnr;
            break;
        }
    }
    if (target == NULL) {
        return FALSE;
    }
    bafhlst_t *bafhlst = &ma->bafhlst;
    list_head_t *list = &bafhlst->af_freelst;
    for (uint_t i = 0;
         mnr < gmdsc.msadrdsc_sz && (adr_t)(target[i].md_phyadr.pf_phyadr << PAGE_4K_SHIFT) < endadr; 
         i++, mnr++) {
        if (target[i].md_flgs.mf_mocty != MF_MOCTY_FREE) {
            continue;
        }
        list_head_t *newnode = &target[i].md_list;
        list_add_tail(newnode, list);
        list = newnode;
        bafhlst->af_objnr++;
        bafhlst->af_freeobjnr++;
    }
    return TRUE;
}

bool_t checkbafhlst(memarea_t *ma) {
    bafhlst_t *bafhlst = &ma->bafhlst;
    list_head_t *head = &bafhlst->af_freelst, *p = head->next;
    int i = 1;
    for (; i < bafhlst->af_freeobjnr; i++) {
        if (p == head || p == NULL) {
            break;
        }
        p = p->next;
    }
    if (i != bafhlst->af_freeobjnr) {
        kerror("index:%d  expected:%d\n", i, bafhlst->af_freeobjnr);
        return FALSE;
    }
    
    return TRUE;
}

bool_t init_memarea_data() {
    // BIOS 中断表占用内存页
    if (!search_occupymsadrdsc(gmdsc.memarea, 0, 0x1000)) {
        return FALSE;
    }
    // 内核占用的内存页
    if (!search_occupymsadrdsc(gmdsc.memarea+1, gmdsc.kernel_start_addr, gmdsc.kernel_end_addr)) {
        return FALSE;
    }
    // 物理区初始化
    if(!build_bafhlst(gmdsc.memarea)) {
        kdebug("physical build bafhlst failed\n");
        return FALSE;
    }
    // 内核区初始化
    if (!build_bafhlst(gmdsc.memarea+1)) {
        kdebug("kernel build bafhlst failed\n");
        return FALSE;
    }
    // 进程区初始化
    if (!build_bafhlst(gmdsc.memarea+2)) {
        kdebug("proc build bafhlst failed\n");
        return FALSE;
    }
    kinfo("physical memarea page num:%d; reserve page num:%d\n", gmdsc.memarea[0].bafhlst.af_objnr, gmdsc.memarea[0].bafhlst.af_resvpg);
    kinfo("kernel memarea page num:%d; reserve page num:%d\n", gmdsc.memarea[1].bafhlst.af_objnr, gmdsc.memarea[1].bafhlst.af_resvpg);
    kinfo("proc memarea page num:%d; reserve page num:%d\n", gmdsc.memarea[2].bafhlst.af_objnr, gmdsc.memarea[2].bafhlst.af_resvpg);

    if (!checkbafhlst(&gmdsc.memarea[0])) {
        kerror("physical not pass check\n");
        return FALSE;
    }
    if (!checkbafhlst(&gmdsc.memarea[1])) {
        kerror("kernel not pass check\n");
        return FALSE;
    }
    if (!checkbafhlst(&gmdsc.memarea[2])) {
        kerror("proc not pass check\n");
        return FALSE;
    }
    return TRUE;
}

msadrdsc_t* alloc_page(uint_t ma_type) {
    memarea_t *ma = NULL;
    switch (ma_type) {
    case MA_TYPE_HWAD:
        ma = gmdsc.memarea;
        break;
    case MA_TYPE_KRNL:
        ma = gmdsc.memarea + 1;
        break;
    case MA_TYPE_PROC:
        ma = gmdsc.memarea + 2;
        break;
    default:
        return 0;
    }
    bafhlst_t *bafhlst = &ma->bafhlst;
    if (bafhlst->af_freeobjnr <= 0) {
        return NULL;
    }

    list_head_t *tmp = bafhlst->af_freelst.next;
    list_del_entry(tmp);

    msadrdsc_t *msadrdsc = (msadrdsc_t*)tmp;
    msadrdsc->md_phyadr.pf_alloc = 1;
    msadrdsc->md_flgs.mf_mocty = ma_type;
    bafhlst->af_allocindx++;

    return msadrdsc;
}
