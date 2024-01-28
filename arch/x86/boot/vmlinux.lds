LOAD_OFFSET = 0xffffffff80000000;
OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(startup)
SECTIONS
{
	. = 16M;
	.boot : {
		*(.text.head)
		*(.data.head)
	}
	
    . += LOAD_OFFSET;
	_text = .;
	.text :  AT(ADDR(.text) - LOAD_OFFSET) {
		*(.text)
		*(.text.*)
	}
	_etext = . ;

	.data : AT(ADDR(.data) - LOAD_OFFSET) {
		_data = . ;
		*(.data)
		*(.data.*)
		_edata = . ;
	}

	_rodata = . ;
	.rodata : AT(ADDR(.rodata) - LOAD_OFFSET) {
		*(.rodata)	 /* read-only data */
		*(.rodata.*)
	}
	_erodata = . ;

	__bss_start = .;
	.bss : AT(ADDR(.bss) - LOAD_OFFSET) {
		_bss = . ;
		*(.bss)
		*(.bss.*)
		*(COMMON)
		. = ALIGN(8);
		_end_before_pgt = . ;
		. = ALIGN(4096);
		pgtable = . ;
		. = . + 4096 * 6;
		_ebss = .;
	}
	__bss_stop = .;
	_end = .;
}