#include "keyboard.h"
#include "types.h"
#include "ptrace.h"
#include "kprint.h"

static struct keyboard_inputbuffer *p_kb = NULL;
static int shift_l = 0, shirt_r = 0, ctrl_l = 0, ctrl_r = 0, alt_l = 0, alt_r = 0;

void parse_keycode(unsigned char x) {
    int make = (x & FLAG_BREAK)? 0: 1, column = 0;
    if (shift_l || shirt_r) {
        column = 1;
    }
    unsigned int *keyrow = &keycode_map_normal[(x & 0x7F)*MAP_COLS];
    int key = keyrow[column];
    switch (x & 0x7F) {
        case 0x2A:  // shift left
            shift_l = make;
            key = 0;
            break;

        case 0x36:  // shift right
            shirt_r = make;
            key = 0;
            break;
        
        default:
            if (!make) key = 0;
            break;
    }
    if (key) {
        kprintf("%c", (char)key);
    }
}

void keyboard_handler(unsigned long nr, unsigned long r) {
    struct pt_regs *regs = (struct pt_regs*) r;
    unsigned char x = io_in8(0x60);
    parse_keycode(x);
}