OUTPUT_FORMAT("elf64-x86-64","elf64-x86-64","elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)

ENTRY(_start)
SECTIONS
{
    . = 32M;
    _start_kernel = .;
    .boot : {
        *(.bs*)
        . = ALIGN(4096);
    }

    VMA = 0xffff800000000000;
    . += VMA;
    start_text = .;
    .text (start_text): AT(start_text - VMA) { *(.text) }
    end_text = .;

    . = ALIGN(32768);
    start_data = .;
    .data (start_data): AT(start_data - VMA) { *(.data) }
    end_data = .;

    . = ALIGN(32768);
    start_rodata = .;
    .rodata (start_rodata): AT(start_rodata - VMA) { *(.rodata) *(.rodata.*) }
    end_rodata = .;

    . = ALIGN(32768);
    .data.init_task : { *(.data.init_task) }

    . = ALIGN(32768);
    start_bss = .;
    .bss (start_bss): AT(start_bss - VMA) { *(.bss) }
    end_bss = .;

    _end_kernel = . - VMA;

    /DISCARD/ : {
		*(.comment)
        *(.eh_frame)
	}
}