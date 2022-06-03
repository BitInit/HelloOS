ASFLAGS = --64
CFLAGS = -E -I../../../lib/include --static -fno-pie -fno-builtin -fno-stack-protector -m64
BOOT_OBJS = multiboot2.o

all: clean build

build: $(BOOT_OBJS)

multiboot2.o: multiboot2.S
	@gcc $(CFLAGS) $< > multiboot2.s~
	@as $(ASFLAGS) -o $@ multiboot2.s~

clean:
	@rm -rf *.s~ *.o
