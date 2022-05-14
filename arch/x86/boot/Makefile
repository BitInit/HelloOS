CFLAGS = --static -I../include -fno-pie -fno-builtin -m32 -c -fno-stack-protector
IMG_HEADER_OBJS = imgheader.o initheader.o printk.o kstring.o
HD_INFO_OBJS = hardwareinfo.o

IMG_HEADER ?= imgHeader.bin
HD_INFO_CHECK ?= hardwareinfocheck.bin

all: clean build

build: imgHeader hardwareInfo

hardwareInfo: $(HD_INFO_OBJS)
	@ld -s -n -o hardwareInfo.elf $^ -T hardwareinfo.lds
	@objcopy -S -O binary hardwareInfo.elf $(HD_INFO_CHECK)

imgHeader: $(IMG_HEADER_OBJS)
	@ld -s -n -o imgHeader.elf $^ -T imgheader.lds
	@objcopy -S -O binary imgHeader.elf $(IMG_HEADER)

imgheader.o: imgheader.asm
	@nasm -f elf $< -o $@

%.o: %.c
	@gcc $(CFLAGS) $< -o $@

clean:
	@rm -rf *.o *.bin *.elf