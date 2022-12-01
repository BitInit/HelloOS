# HelloOS
基于 x86_64 指令集的实验性操作系统。

## 构建
目前基于 debian/ubuntu 进行构建：

1. 安装依赖

```bash
$ apt install -y make gcc grub git
```

2. 代码拉取 

```bash
$ git clone git@github.com:BitInit/HelloOS.git
```

3. 构建镜像

```bash
$ cd HelloOS

$ make
```

在目录项目主目录下，`HelloOS.iso` 为构建好的镜像文件，`kernel.elf` 为构建好的内核文件。

## 进度

- [x] grub 启动引导

- [x] 基于位图的内存管理

- [ ] 内存管理改造成伙伴算法

- [ ] 支持 slab

- [x] 基于 8259A PIC 的中断处理

- [ ] 基于 APIC 的中断处理

- [x] 基于 8042 的键盘驱动

- [x] 基于 `sysenter` 与 `sysexit` 指令的系统调用

- [ ] posix 支持

- [ ] 进程调度

- [ ] 多核处理器支持

- [ ] 磁盘驱动

- [ ] 支持 FAT32 文件系统

- [ ] shell 命令解析器

- [ ] 移植 ls/cat/cp 等常用工具

- [ ] 支持 risc-v 指令集


## 参考资料

* [《x86汇编语言：从实模式到保护模式》](https://book.douban.com/subject/20492528/)
* [《一个64位操作系统的设计与实现》](https://book.douban.com/subject/30222325/)