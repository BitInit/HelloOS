#include "keyboard.h"
#include "types.h"
#include "ptrace.h"
#include "kprint.h"

static struct keyboard_inputbuffer *p_kb = NULL;
static int shift_l, shirt_r, ctrl_l, ctrl_r, alt_l, alt_r;

void parse_keycode(unsigned char x) {
    int make = (x & FLAG_BREAK)? 0: 1;
    if (!make) {
        return;
    }
    unsigned int *keyrow = &keycode_map_normal[(x & 0x7F)*MAP_COLS];
    int key = keyrow[0];
    kdebug("%c\n", (char)key);
}

void keyboard_handler(unsigned long nr, unsigned long r) {
    struct pt_regs *regs = (struct pt_regs*) r;
    unsigned char x = io_in8(0x60);
    parse_keycode(x);
    // kprintf("keyboard: %#08x\n", x);
}