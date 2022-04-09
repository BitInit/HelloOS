rm boot.bin
rm loader.bin
nasm ../boot.asm -o boot.bin
nasm ../loader.asm -o loader.bin
dd if=boot.bin of=boot.img bs=512 count=1 conv=notrunc
bochs
