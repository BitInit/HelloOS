OUTPUT_FORMAT("elf64-x86-64","elf64-x86-64","elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)

/*OUTPUT_ARCH(i386)
OUTPUT_FORMAT(elf32-i386)*/

ENTRY(_start)
SECTIONS
{
    . = 1M;
    .text : { *(.text) }

    .data : ALIGN(4) { *(.data) }

    .rodata : ALIGN(4) { *(.rodata) *(.rodata.*) }

    .bss : ALIGN(4) { *(.bss) }
}