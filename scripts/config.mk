ASFLAGS=--64
CFLAGS = -c -I$(BASE_DIR)/include -mcmodel=large --static -fno-pie -fno-builtin -fno-stack-protector -m64

c_srcs = $(wildcard ./*.c)
c_srcs_obj = $(patsubst %.c, %.o, $(c_srcs))

%.o: %.c
	@echo "compile $<"
	$(HIDE)$(CC) $(CFLAGS) $(DEBUG) $< -o $@

%.o: %.S
	@echo "compile $<"
	$(HIDE)$(CC) -E $(CFLAGS) $< > $<.i
	$(HIDE)$(AS) $(ASFLAGS) -o $@ $<.i

$(c_srcs_obj): $(c_srcs)