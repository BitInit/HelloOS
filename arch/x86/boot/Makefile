CFLAGS = --static -I../include -fno-pie -fno-builtin -m32 -c -fno-stack-protector
OBJS = imgheader.o initheader.o printk.o string.o

IMG_HEADER ?= imgHeader.bin

all: clean build

build: $(OBJS)
	@ld -s -n -o imgHeader.elf $^ -T imgheader.lds
	@objcopy -S -O binary imgHeader.elf $(IMG_HEADER)

imgheader.o: imgheader.asm
	@nasm -f elf $< -o $@

%.o: %.c
	@gcc $(CFLAGS) $< -o $@

clean:
	@rm -rf *.o *.bin *.elf