X86_BOOT = arch/x86/boot
LIB = lib
KERNEL = kernel
OBJS = $(X86_BOOT)/multiboot2.o $(LIB)/printk.o $(KERNEL)/setup.o $(LIB)/string.o

build: 
	make -C $(X86_BOOT) all
	make -C $(LIB) all
	make -C $(KERNEL) all
	ld -b elf32-i386 $(OBJS) -z muldefs -s -n -o kernel.elf  -T kernel.lds

clean:
	@make -C $(X86_BOOT) clean
	@rm -rf kernel.elf
