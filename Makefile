MAKEFLAGS = --no-print-directory
BIMG_DIR = ./tool/bimg
BIMG = bimg
BOOT_DIR = ./arch/x86/boot
IMG_HEADER = imgheader.bin
HD_INFO_CHECK = hardwareinfocheck.bin
HELLO_OS = HelloOS
export MAKEFLAGS IMG_HEADER BIMG HD_INFO_CHECK

all: clean img

img: bimg boot
	@$(BIMG_DIR)/$(BIMG) -h $(BOOT_DIR)/$(IMG_HEADER) -f $(BOOT_DIR)/$(HD_INFO_CHECK) -o $(HELLO_OS)
	@bash build_iso.sh

boot:
	@make -s -C $(BOOT_DIR)

bimg:
	@make -s -C $(BIMG_DIR)

clean:
	@rm -rf $(HELLO_OS) *.iso
	@make -C $(BIMG_DIR) clean
	@make -C $(BOOT_DIR) clean

cleanall : clean
	@rm -rf $(HELLO_OS)
