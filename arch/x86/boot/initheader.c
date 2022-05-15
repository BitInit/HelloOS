#include "printk.h"
#include "ldrimg.h"
#include "kstring.h"

void exit();
void cp_file(ldrimg_t *ldrimg, char *name, char *dst);

void inithead_entry() {
    clear_screen();

    ldrimg_t *ldrimg = (ldrimg_t*) IMG_BASE;
    if (ldrimg->img_magic != LDRIMG_MAGIC) {
        err_printf("image file format error");
        exit();
    }
    
    cp_file(ldrimg, MACHINE_CHECK_BIN, (char*)MACHINE_CHECK_ADDR);
    cp_file(ldrimg, KERNEL_BIN, (char*)KERNEL_ADDR);
    return;
}

void cp_file(ldrimg_t *ldrimg, char *name, char *dst) {
    uint32 file_num = ldrimg->fhd_num, fhd_size = 0;
    char *src = NULL;
    for (int32 i = 0; i < file_num; i++) {
        fhdsc_t *fhdsc = (fhdsc_t*) (LDRFILE_BASE + i*sizeof(fhdsc_t));
        if (!strcmp(name, fhdsc->fhd_name)) {
            src = (char*)(IMG_BASE + fhdsc->fhd_posoffset);
            fhd_size = fhdsc->fhd_size;
            break;
        }
    }
    if (!src) {
        err_printf("[error] can't find %s\n", name);
        exit();
    }
    printf("[info] load %s successfully\n", name);
    
    strncpy(dst, src, fhd_size);
    return;
}

void exit() {
    while (TRUE) ;
    return;
}