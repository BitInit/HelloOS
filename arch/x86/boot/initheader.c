#include "printk.h"
#include "ldrimg.h"

void sleep(uint32 times) {
    while (times--) ;
}

void inithead_entry() {
    clear_screen();

    for (int i = 0; i < 10000; i++) {
        err_printf("%d - %s! hex:%x\n", i, "hello world", i);
        sleep(300000);
    }
    
    return;
}