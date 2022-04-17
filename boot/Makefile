ASM = nasm

all : clean build

build : $(BOOT_BIN) $(LOADER_BIN)

%.bin : %.asm
	@$(ASM) $< -o $@

clean :
	@rm -rf $(BOOT_BIN) $(LOADER_BIN)

