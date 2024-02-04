#ifndef _ASM_X86_DESC_H
#define _ASM_X86_DESC_H

#include <os/types.h>
#include <os/string.h>

#include <asm/segment.h>

#define PTR_LOW(x) ((unsigned long long)(x) & 0xFFFF)
#define PTR_MIDDLE(x) (((unsigned long long)(x) >> 16) & 0xFFFF)
#define PTR_HIGH(x) ((unsigned long long)(x) >> 32)

extern gate_desc idt_table[];

struct gate_struct64 {
	uint16_t offset_low;
	uint16_t segment;
	unsigned ist : 3, zero0 : 5, type : 5, dpl : 2, p : 1;
	uint16_t offset_middle;
	uint32_t offset_high;
	uint32_t zero1;
} __attribute__((packed));

typedef struct gate_struct64 gate_desc;

enum {
	GATE_INTERRUPT = 0xE,
	GATE_TRAP = 0xF,
	GATE_CALL = 0xC,
	GATE_TASK = 0x5,
};

static inline void pack_gate(gate_desc *gate, unsigned type, unsigned long func,
			     unsigned dpl, unsigned ist, unsigned seg) {
	gate->offset_low = PTR_LOW(func);
	gate->segment = __KERNEL_CS;
	gate->ist = ist;
	gate->p = 1;
	gate->dpl = dpl;
	gate->zero0 = 0;
	gate->zero1 = 0;
	gate->type = type;
	gate->offset_middle = PTR_MIDDLE(func);
	gate->offset_high = PTR_HIGH(func);
}

static inline void write_idt_entry(gate_desc *idt, int entry,
					  const gate_desc *gate) {
	memcpy(&idt[entry], gate, sizeof(*gate));
}

static inline void _set_gate(int gate, unsigned type, void *addr,
			     unsigned dpl, unsigned ist, unsigned seg) {
	gate_desc s;
	pack_gate(&s, type, (unsigned long)addr, dpl, ist, seg);
	/*
	 * does not need to be atomic because it is only done once at
	 * setup time
	 */
	write_idt_entry(idt_table, gate, &s);
}

static inline void set_intr_gate(unsigned int n, void *addr) {
	_set_gate(n, GATE_INTERRUPT, addr, 0, 0, __KERNEL_CS);
}

#endif