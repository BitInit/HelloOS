Q: **lretq 模拟远程调用，方法：**
```assembly
pushq 0x08 # cs
pushq %rax # ip
lretq

# 但有时为
pushq %rax # ip
lretq
```

**如何识别是否设置了 cs 寄存器？**

A：

---

