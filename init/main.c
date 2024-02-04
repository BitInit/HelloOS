#include <os/init.h>

void start_kernel() {

    setup_arch();

    __asm__ __volatile__("hlt");
}

