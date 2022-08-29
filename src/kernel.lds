OUTPUT_FORMAT("elf64-x86-64","elf64-x86-64","elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)

ENTRY(_start)
SECTIONS
{
    . = 32M;
    .boot : {
        *(.bs*)
        . = ALIGN(4096);
    }

    VMA = 0xffff800000000000;
    . += VMA;
    _start_kernel = . - VMA;
    start_text = .;
    .text (start_text): AT(start_text - VMA) { *(.text) }

    . = ALIGN(32768);
    start_data = .;
    .data (start_data): AT(start_data - VMA) { *(.data) }

    . = ALIGN(32768);
    start_rodata = .;
    .rodata (start_rodata): AT(start_rodata - VMA) { *(.rodata) *(.rodata.*) }

    . = ALIGN(32768);
    start_bss = .;
    .bss (start_bss): AT(start_bss - VMA) { *(.bss) }

    _end_kernel = . - VMA;

    /DISCARD/ : {
		*(.comment)
        *(.eh_frame)
	}
}