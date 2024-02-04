#include <asm/bootparam.h>

#include <os/init.h>

struct boot_params boot_params;

void memory_setup() {
    machine_specific_memory_setup();
}

void setup_arch() {
    memory_setup();
}