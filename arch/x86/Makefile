export ASFLAGS=--64
export CFLAGS = -I$(BASE_DIR)/include --static -fno-pie -fno-builtin -fno-stack-protector -m64

x86_srcs = boot kernel

build:
	$(HIDE)for dir in $(x86_srcs); do \
		make -C $$dir || exit 1; \
	done