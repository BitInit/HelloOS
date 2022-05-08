#include "printk.h"

void sleep(uint32 times) {
    while (times--) ;
}

void inithead_entry() {
    clear_screen();

    for (int i = 0; i < 10000; i++) {
        printf("%d\n", i);
        sleep(300000);
    }
    return;
}