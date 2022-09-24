#ifndef __HELLOOS_IRQ_H
#define __HELLOOS_IRQ_H

void init_interrupt();

void do_IRQ(unsigned long regs,unsigned long nr);

void IRQ_nothing();

#endif