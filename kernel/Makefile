
KERNEL_ELF = kernel.elf
objects = head.o main.o printk.o

all : clean build

build : $(objects)
	@ld -b elf64-x86-64 -z muldefs -o $(KERNEL_ELF) $(objects) -T kernel.lds
	@objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary $(KERNEL_ELF) $(KERNEL_BIN)

main.o : main.c lib.h
	@gcc --static -mcmodel=large -fno-builtin -m64 -c $< -o $@ -fno-stack-protector

printk.o : printk.c lib.h
	@gcc --static -mcmodel=large -fno-builtin -m64 -c $< -o $@ -fno-stack-protector

head.o : head.S
	@as --64 -o $@ $<

clean :
	@rm -rf $(KERNEL_BIN) $(KERNEL_ELF) *.o *.o~
