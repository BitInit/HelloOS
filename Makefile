MAKEFLAGS = --no-print-directory

SRC := arch/x86/boot kernel mm drivers

all: clean iso

iso: build
	ld -b elf64-x86-64 -z muldefs -o kernel.elf -T link.lds $(shell find $(SRC) -name "*.o")
	@bash build_iso.sh

build: $(SRC)
	@for s in $(SRC); do \
		make -C $$s build || exit 1; \
	done

clean: $(SRC)
	@for s in $(SRC); do \
		make -C $$s clean || exit 1; \
	done
	rm -rf *.elf *.iso
