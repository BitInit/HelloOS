#include <asm-generic/sections.h>

#include <os/string.h>
#include <os/start_kernel.h>
#include <os/kernel.h>

void clear_bss() {
    // memset(__bss_start, 0,
	//        (unsigned long) __bss_stop - (unsigned long) __bss_start);
}


void x86_64_start_kernel() {
    early_printk("hello world");

    start_kernel();
}