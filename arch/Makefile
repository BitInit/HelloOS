kernel_target=kernel.elf
SRCS = $(BASE_DIR)/init $(BASE_DIR)/kernel $(BASE_DIR)/mm
ifeq ($(CONFIG_x86_64), y)
	SRCS += $(BASE_DIR)/arch/x86
	lds = $(BASE_DIR)/arch/x86/boot/vmlinux.lds
else
	$(HIDE)echo "unkown arch..."
endif

$(TARGET_ISO): clean $(kernel_target)
	$(HIDE)rm -rf build_dir
	$(HIDE)mkdir -p build_dir/boot/grub
	$(HIDE)cp $(kernel_target) build_dir/boot

	$(HIDE)echo 'set timeout=1' > build_dir/boot/grub/grub.cfg
	$(HIDE)echo 'set default=0' >> build_dir/boot/grub/grub.cfg
	$(HIDE)echo 'menuentry "HelloOS" {' >> build_dir/boot/grub/grub.cfg
	$(HIDE)echo '	multiboot2 /boot/kernel.elf "HelloOS"' >> build_dir/boot/grub/grub.cfg
	$(HIDE)echo '}' >> build_dir/boot/grub/grub.cfg

	$(HIDE)grub-mkrescue -o $(TARGET_ISO) build_dir
	$(HIDE)rm -rf build_dir

$(kernel_target): compile
	$(HIDE)ld -b elf64-x86-64 -z muldefs -o $(kernel_target) -Map=./kernel.map -T $(lds) $(shell find $(SRCS) -name "*.o") 

compile:
	$(HIDE)for dir in $(SRCS); do \
		make -C $$dir || exit 1; \
	done

clean:
	$(HIDE)rm -rf $(kernel_target) $(TARGET_ISO) kernel.map

cleanall: clean
	$(HIDE)for dir in $(SRCS); do \
		make -C $$dir clean || exit 1; \
	done