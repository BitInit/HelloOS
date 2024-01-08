export ASFLAGS=--64
export CFLAGS = -I$(BASE_DIR)/include --static -fno-pie -fno-builtin -fno-stack-protector -m64

x86_srcs = $(wildcard ./boot/*.c) $(wildcard ./kernel/*.c) $(wildcard ./*.c)
x86_objs = $(patsubst %.c, %.o, $(x86_srcs))

x86_asm_srcs = $(wildcard ./boot/*.S) $(wildcard ./kernel/*.S)
x86_asm_tmp_srcs = $(patsubst %.S, %.i, $(x86_asm_srcs))
x86_asm_objs = $(patsubst %.S, %.o, $(x86_asm_srcs))

build: $(x86_asm_objs) $(x86_objs)

$(x86_objs): $(x86_srcs)
$(x86_asm_objs): $(x86_asm_srcs)

%.o: %.c
	@echo "compile $<"
	$(CC) $(CFLAGS) $(DEBUG) $< -o $@

%.o: %.S
	@echo "compile $<"
	$(HIDE)$(CC) -E $(CFLAGS) $< > $<.i
	$(HIDE)$(AS) $(ASFLAGS) -o $@ $<.i

clean:
	$(HIDE)rm -rf $(x86_objs) $(x86_asm_objs) $(x86_asm_tmp_srcs)

