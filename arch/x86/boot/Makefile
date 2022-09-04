ASFLAGS = --64
CFLAGS = -I../../../include --static -fno-pie -fno-builtin -fno-stack-protector -m64
BOOT_OBJS = multiboot2.o

all: clean build

build: $(BOOT_OBJS)

multiboot2.o: multiboot2.S
	@gcc -E $(CFLAGS) $< > multiboot2.i
	@as $(ASFLAGS) -o $@ multiboot2.i

%.o: %.c
	@gcc -c $(CFLAGS) -o $@ $<

clean:
	@rm -rf *.i *.o
