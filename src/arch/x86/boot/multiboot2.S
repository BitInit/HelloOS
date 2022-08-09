#define ASM_FILE 1
#include "multiboot2.h"

#ifdef KERN_VBE_MODE
#define FRAMEBUFFER_WIDTH 1024
#define FRAMEBUFFER_HEIGHT 768
#define FRAMEBUFFER_BPP 24
#endif /* KERN_VBE_MODE */

.code32
.section .bs
.globl _start
_start:
    jmp multiboot_entry

.align 8
multiboot_header:
    .long MULTIBOOT2_HEADER_MAGIC                  # magic number (multiboot 2)
    .long MULTIBOOT_ARCHITECTURE_I386              # architecture 0 (protected mode i386)
    .long multiboot_header_end - multiboot_header  # header length
    # checksum
    .long -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + (multiboot_header_end - multiboot_header))
    # insert optional multiboot tags here

#ifdef KERN_VBE_MODE
    .align 8
    tag_framebuffer:
        .short MULTIBOOT_HEADER_TAG_FRAMEBUFFER
        .short MULTIBOOT_HEADER_TAG_OPTIONAL
        .long  tag_framebuffer_end - tag_framebuffer
        .long  FRAMEBUFFER_WIDTH    # screen width
        .long  FRAMEBUFFER_HEIGHT   # screen height
        .long  FRAMEBUFFER_BPP      # bit per pixel
    tag_framebuffer_end:
#endif /* KERN_VBE_MODE */

    # required end tag
    .align 8
    .short MULTIBOOT_HEADER_TAG_END # type
    .short 0                        # flags
    .long  8                        # size
multiboot_header_end:

multiboot_entry:
    cli
    # reset EFLAGS
    pushl $0
    popf

    # the magic value
    mov %eax, mb2_magic
    # the pointer to the Multiboot information structure
    mov %ebx, mb2_info

    # 允许 PAE
    mov %cr4, %eax
    or $(1<<5), %eax
    mov %eax, %cr4

    /* 设置临时页表 */
    # 顶级页目录表
    mov $pml4, %eax
    mov $pdpt, %ebx
    or $0x03, %ebx
    mov %ebx, 0(%eax)
    mov %ebx, 256(%eax)
    # 页目录指针表
    mov $pdpt, %eax
    mov $pd, %ebx
    or $0x03, %ebx
    mov %ebx, 0(%eax)
    # 页目录表
    mov $512, %ecx
    mov $pd, %eax
    mov $0x83, %ebx
.pd_loop:
    mov %ebx, 0(%eax)
    add $0x200000, %ebx
    add $0x08, %eax
    loop .pd_loop

    # 设置 CR3
    mov $pml4, %eax
    mov %eax, %cr3

    # 切换到 long 模式
    mov $0xC0000080, %ecx
    rdmsr
    or $(1<<8), %eax
    wrmsr

    # 开启分页
    mov %cr0, %eax
    or $(1<<31), %eax
    mov %eax, %cr0

    # 重新设置 GDT
    mov $gdt64_pointer64, %eax
    lgdt 0(%eax)
    # 跳转到 64 位代码执行
    jmp $0x8, $_start64
    hlt
    ret

.extern kernel_start
.code64
_start64:
    # 更新
    mov $0x10, %rax
    mov %rax, %ds
    mov %rax, %es
    mov %rax, %fs
    mov %rax, %gs
    mov %rax, %ss
    # 设置栈地址
    mov $STACK_TOP, %rsp
    # 栈地址按照 4096 字节对齐
    and $0xFFFFFFFFFFFFF000, %rsp
    # 帧指针修改为 0
    mov $0, %rbp

    # x86_64 位，函数调用通过寄存器传参
    movq mb2_info, %rsi
    movq mb2_magic, %rdi
    # call kernel_start

    hlt
    ret

/*
 * 临时页表，4KB/页
 */
.section .bs.data
.align 0x1000
pml4: /* 顶级页目录表 */
    .skip 0x1000
pdpt: /* 页目录指针表*/
    .skip 0x1000
pd: /* 页目录表 */
    .skip 0x1000

.align 16
gdt64:
null_desc:
    .short 0xFFFF
    .short 0
    .byte 0
    .byte 0
    .byte 0
    .byte 0
code_desc:
    .short 0
    .short 0
    .byte 0
    .byte 0x9A
    .byte 0x20
    .byte 0
data_desc:
    .short 0
    .short 0
    .byte 0
    .byte 0x92
    .byte 0
    .byte 0
user_code_desc:
    .short 0
    .short 0
    .byte 0
    .byte 0xFA
    .byte 0x20
    .byte 0
user_data_desc:
    .short 0
    .short 0
    .byte 0
    .byte 0xF2
    .byte 0
    .byte 0
gdt64_pointer64:
    .short gdt64_pointer64-gdt64-1
    .quad gdt64

mb2_magic: .quad 0x00
mb2_info: .quad 0x00

.section .bs.bss
.align 0x1000
STACK:
    .skip 4096 * 4
STACK_TOP:
