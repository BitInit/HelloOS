#define ASM_FILE
#include <asm/multiboot2.h>

.section .text.head
.global startup
.extern startup_64

.code32
startup:
    jmp startup_64

.align 8
multiboot_header:
    .long MULTIBOOT2_HEADER_MAGIC                  /* magic number (multiboot 2) */
    .long MULTIBOOT_ARCHITECTURE_I386              /* architecture 0 (protected mode i386) */
    .long multiboot_header_end - multiboot_header  /* header length */
    .long -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + (multiboot_header_end - multiboot_header))

    /* required end tag */
    .align 8
    .short MULTIBOOT_HEADER_TAG_END # type
    .short 0                        # flags
    .long  8                        # size
multiboot_header_end:
