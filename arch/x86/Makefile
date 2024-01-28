-include $(BASE_DIR)/scripts/config.mk

x86_srcs = $(wildcard ./boot/*.c) $(wildcard ./kernel/*.c) $(wildcard ./*.c)
x86_objs = $(patsubst %.c, %.o, $(x86_srcs))

x86_asm_srcs = $(wildcard ./boot/*.S) $(wildcard ./kernel/*.S)
x86_asm_tmp_srcs = $(patsubst %.S, %.i, $(x86_asm_srcs))
x86_asm_objs = $(patsubst %.S, %.o, $(x86_asm_srcs))

compile: $(x86_asm_objs) $(x86_objs)

$(x86_objs): $(x86_srcs)
$(x86_asm_objs): $(x86_asm_srcs)

clean:
	$(HIDE)rm -rf $(x86_objs) $(x86_asm_objs) $(x86_asm_tmp_srcs)

