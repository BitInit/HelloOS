
HEAD = head.s
KERNEL_ELF = kernel.elf

all : clean build
	@echo "hhh"

build : head.o main.o
	@ld -b elf64-x86-64 -o $(KERNEL_ELF) head.o main.o -T kernel.lds
	@objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary $(KERNEL_ELF) $(KERNEL_BIN)

%.o : %.c
	@gcc -mcmodel=large -fno-builtin -m64 -c $< -o $@

head.o : head.S
	@gcc -E $< > $(HEAD)
	@as --64 -o $@ $(HEAD)

clean :
	@rm -rf $(KERNEL_BIN) $(KERNEL_ELF) *.s *.s~ *.o *.o~