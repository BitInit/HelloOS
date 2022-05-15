CFLAGS = --static -I../include -fno-pie -fno-builtin -m32 -c -fno-stack-protector
IMG_HEADER_OBJS = imgheader.o initheader.o printk.o kstring.o
MACHINE_INFO_OBJS = machineinfo.o

IMG_HEADER ?= imgHeader.bin
MACHINE_INFO_CHECK ?= machineinfocheck.bin

all: clean build

build: imgHeader machineInfo

machineInfo: $(MACHINE_INFO_OBJS)
	@ld -s -n -o machineinfo.elf $^ -T machineinfo.lds
	@objcopy -S -O binary machineinfo.elf $(MACHINE_INFO_CHECK)

imgHeader: $(IMG_HEADER_OBJS)
	@ld -s -n -o imgHeader.elf $^ -T imgheader.lds
	@objcopy -S -O binary imgHeader.elf $(IMG_HEADER)

imgheader.o: imgheader.asm
	@nasm -f elf $< -o $@

%.o: %.c
	@gcc $(CFLAGS) $< -o $@

clean:
	@rm -rf *.o *.bin *.elf