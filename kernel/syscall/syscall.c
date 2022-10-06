#include "syscall.h"
#include "kprint.h"

#define MAX_SYSTEM_CALL_NR 128

unsigned long no_system_call(struct pt_regs *regs) {
    kinfo("no_system_call is calling, NR:%#04x\n", regs->rax);
    return -1;
}

typedef unsigned long (*system_call_t)(struct pt_regs *regs);
system_call_t system_call_table[MAX_SYSTEM_CALL_NR] = {
    [0 ... MAX_SYSTEM_CALL_NR-1] = no_system_call
};

unsigned long system_call_function(struct pt_regs *regs) {
    return system_call_table[regs->rax](regs);
}