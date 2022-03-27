#!/bin/bash

HOST_OS=`uname -s`
if [ "$HOST_OS" != "Linux" ]; then
    echo "Currently only supports Linux system."
    exit
fi

GRUB_PATH="$(dirname $(which grub-file))"
# KERNEL_IMAGE="./bin/HelloOS_kernel.elf"

# creating root directory of iso and removing all files
ISO_PATH="$(dirname $0)/iso"
mkdir -p $ISO_PATH
rm -rf ${ISO_PATH}/*

ISO_BOOT="${ISO_PATH}/boot"
ISO_BOOT_GRUB="${ISO_BOOT}/grub"
mkdir -p $ISO_BOOT_GRUB

# fake
touch ${ISO_BOOT}/HelloOS_kernel.elf

# set grub.cfg
touch "${ISO_BOOT_GRUB}/grub.cfg"
echo 'set timeout=15
set default=0
menuentry "HelloOS" {
    multiboot2 /boot/HelloOS_kernel.elf "KERNEL_ELF"
}' >${ISO_BOOT_GRUB}/grub.cfg

${GRUB_PATH}/grub-mkrescue -o HelloOS.iso ${ISO_PATH}

