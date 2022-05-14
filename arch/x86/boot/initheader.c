#include "printk.h"
#include "ldrimg.h"

#define IMG_BASE 0x8000000
#define LDRIMG_BASE (IMG_BASE + HEADER_SIZE)

void sleep(uint32 times) {
    while (times--) ;
}

void inithead_entry() {
    clear_screen();
    int i = 0;
    printf("func addr:%x\n", inithead_entry);

    ldrimg_t *ldrimg = (ldrimg_t*) IMG_BASE;
    printf("magic: %x\n", ldrimg->img_magic);
    printf("magic addr:%x\n", &ldrimg->img_magic);
    printf("i addr:%x\n", &i);

    printf("sizeof: %d\n", sizeof(char*));
    return;
}