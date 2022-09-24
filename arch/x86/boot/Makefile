ASFLAGS = --64
CFLAGS = -I../../../include --static -fno-pie -fno-builtin -fno-stack-protector -m64
OBJS = multiboot2.o entry.o

all: clean build

build: $(OBJS)

multiboot2.o: multiboot2.S
	@gcc -E $(CFLAGS) $< > multiboot2.i
	@as $(ASFLAGS) -o $@ multiboot2.i

entry.o: entry.S
	@gcc -E $(CFLAGS) $< > entry.i
	@as $(ASFLAGS) -o $@ entry.i

%.o: %.c
	@gcc -c $(CFLAGS) -o $@ $<

clean:
	@rm -rf *.i *.o
