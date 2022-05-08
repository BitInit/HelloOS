ENTRY(_start)
OUTPUT_ARCH(i386)
OUTPUT_FORMAT(elf32-i386)
SECTIONS
{
    .text : { *(.text) }

    .data : ALIGN(4) { *(.data) }

    .rodata : ALIGN(4) { *(.rodata) *(.rodata.*) }

    .bss : ALIGN(4) { *(.bss) }
}