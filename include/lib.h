#ifndef _HELLOOS_LIB_H
#define _HELLOOS_LIB_H

#define cli() __asm__ __volatile__("cli \n\t":::"memory")
#define sti() __asm__ __volatile__("sti \n\t":::"memory")
#define nop() __asm__ __volatile__("nop \n\t")
#define port_insw(port, buffer, nr)	\
    __asm__ __volatile__("cld;rep;insw;mfence;"::"d"(port),"D"(buffer),"c"(nr):"memory")

#define port_outsw(port,buffer,nr)	\
__asm__ __volatile__("cld;rep;outsw;mfence;"::"d"(port),"S"(buffer),"c"(nr):"memory")

#define container_of(ptr, type, member) ({   \
    typeof(((type*)0)->member) *_p = (ptr);  \
    (type*)((unsigned long)_p - (unsigned long)&(((type*)0)->member));   \
})

void io_out8(unsigned short port, unsigned char value);

unsigned char io_in8(unsigned short port);

void io_out16(unsigned short port, unsigned short value);

unsigned short io_in16(unsigned short port);

void io_out32(unsigned short port, unsigned int value);

unsigned int io_in32(unsigned short port);

/**
 * wrmsr（write model specific）写 msr 寄存器。
 * msr 寄存器（一组64位的寄存器）：EDX:EAX 分别组成该寄存器的高 32 位和低 32 位，ECX 指定地址。
 *                         wrmsr和rdmsr必须在 0 特权级或实模式下执行。
 */
static inline void wrmsr(unsigned long address,unsigned long value) {
	__asm__ __volatile__("wrmsr	\n\t"::"d"(value >> 32),"a"(value & 0xffffffff),"c"(address):"memory");	
}

#endif