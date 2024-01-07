OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(startup)
SECTIONS
{
    . = 0;
	.text.head : {
		_head = . ;
		*(.text.head)
		_ehead = . ;
	}

    
}