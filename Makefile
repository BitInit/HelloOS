
BOOT_DIR = boot
HELLO_OS_IMG = HelloOS.img
TMP_DIR = tmp
export BOOT_BIN = boot.bin
export LOADER_BIN = loader.bin

all : clean build

build : $(BOOT_DIR)
	@if [ ! -e $(HELLO_OS_IMG) ]; then \
		bximage -fd=1.44M -q -mode=create $(HELLO_OS_IMG); \
	fi
	@make -C $<
	@dd if=$(BOOT_DIR)/$(BOOT_BIN) of=$(HELLO_OS_IMG) bs=512 count=1 conv=notrunc
	@mkdir -p ./$(TMP_DIR)
	@sudo mount $(HELLO_OS_IMG) ./$(TMP_DIR)/ -t vfat -o loop
	@sudo cp $(BOOT_DIR)/$(LOADER_BIN) ./$(TMP_DIR)
	@sync
	@sudo umount $(TMP_DIR)
	@rm -rf $(TMP_DIR)
	

clean : $(BOOT_DIR)
	@make -C $< clean	

cleanall : clean
	@rm -rf $(HELLO_OS_IMG)
