OUTPUT_FORMAT("elf64-x86-64","elf64-x86-64","elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)

ENTRY(_start)
SECTIONS
{
    . = 32M;
    _start_kernel = .;
    .text : { *(.text) }

    .data : ALIGN(4) { *(.data) }

    .rodata : ALIGN(4) { *(.rodata) *(.rodata.*) }

    .bss : ALIGN(4) { *(.bss) }

    _end_kernel = .;
}