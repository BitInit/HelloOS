export BASE_DIR=$(shell pwd)
export TARGET_ISO=HelloOS.iso
export CONFIG_x86_64=y
export CC=gcc
export AS=as
export HIDE=@
ifneq ($(HIDE),)
	MAKEFLAGS += --no-print-dirctory
endif


HelloOS:
	$(HIDE)make -C arch

clean:
	$(HIDE)make -C arch clean

cleanall:
	$(HIDE)make -C arch cleanall

run:
	qemu-system-x86_64 -m 1G -cdrom arch/$(TARGET_ISO) -nographic