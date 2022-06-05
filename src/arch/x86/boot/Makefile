ASFLAGS = --64
CFLAGS = -I../../../lib/include --static -fno-pie -fno-builtin -fno-stack-protector -m64
BOOT_OBJS = multiboot2.o

all: clean build

build: $(BOOT_OBJS)

multiboot2.o: multiboot2.S
	@gcc -E $(CFLAGS) $< > multiboot2.s~
	@as $(ASFLAGS) -o $@ multiboot2.s~

%.o: %.c
	@gcc -c $(CFLAGS) -o $@ $<

clean:
	@rm -rf *.s~ *.o
