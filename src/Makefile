X86_BOOT = arch/x86/boot
LIB = lib
KERNEL = kernel
export OBJS=$(X86_BOOT)/multiboot2.o $(LIB)/printk.o \
	$(KERNEL)/kernel.o $(KERNEL)/mm/mm.o \
	$(LIB)/string.o

build: 
	make -C $(X86_BOOT) all
	make -C $(LIB) all
	make -C $(KERNEL) all
	ld $(OBJS) -b elf64-x86-64 -z muldefs -o kernel.elf -T kernel.lds

clean:
	@make -C $(X86_BOOT) clean
	@make -C $(KERNEL) clean
	@make -C $(LIB) clean
	@rm -rf kernel.elf
