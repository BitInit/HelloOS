## lretq 模拟远程调用
```assembly
pushq 0x08 # cs
pushq %rax # ip
lretq

# 但有时为
pushq %rax # ip
lretq
```

## 关于 gcc -mcmodel 使用
在编译下面代码时：
``` c
void x86_64_start_kernel() {
    early_printk("hello world");

    start_kernel();
}
```

出现了 `relocation truncated to fit: R_X86_64_32 against .rodata'` 问题，反编译后的目标文件 `objdump -dra xx.o`：
```
000000000000000b <x86_64_start_kernel>:
   b:	f3 0f 1e fa          	endbr64
   f:	55                   	push   %rbp
  10:	48 89 e5             	mov    %rsp,%rbp
  13:	bf 00 00 00 00       	mov    $0x0,%edi
			14: R_X86_64_32	.rodata
  18:	b8 00 00 00 00       	mov    $0x0,%eax
  1d:	e8 00 00 00 00       	call   22 <x86_64_start_kernel+0x17>
			1e: R_X86_64_PLT32	early_printk-0x4
  22:	b8 00 00 00 00       	mov    $0x0,%eax
  27:	e8 00 00 00 00       	call   2c <x86_64_start_kernel+0x21>
			28: R_X86_64_PLT32	start_kernel-0x4
  2c:	90                   	nop
  2d:	5d                   	pop    %rbp
  2e:	c3                   	ret
```
字符串常量 `hello world` 存储在 `.rodata` 段中，但使用的是 `R_X86_64_32`，解决方法是在编译该源文件时，使用 `gcc -mcmodel xx`。
