#ifndef _HELLOOS_KEYBOARD_H
#define _HELLOOS_KEYBOARD_H

#include "lib.h"

#define PORT_KB_DATA 0x60
#define PORT_KB_STATUS 0x64
#define PORT_KB_CMD 0x64

#define KBCMD_WRITE_CMD 0x60
#define KBCMD_READ_CMD 0x20
#define KB_INIT_MODE 0x47

#define KB_BUF_SIZE 100

struct keyboard_inputbuffer {
    unsigned char *p_head;
    unsigned char *p_tail;
    int count;
    unsigned char buf[KB_BUF_SIZE];
};

#define KBSTATUS_IBF 0x02
#define KBSTATUS_OBF 0x01
#define wait_kb_write() while(io_in8(PORT_KB_STATUS) & KBSTATUS_IBF)
#define wait_kb_read() while(io_in8(PORT_KB_STATUS) & KBSTATUS_OBF)

#define NR_SCAN_CODES 0x80
#define MAP_COLS 2
#define PAUSEBREAK 1
#define PRINTSCREEN 2
#define OTHERKEY 4
#define FLAG_BREAK 0x80

unsigned char pausebreak_scode[] = {0xE1, 0x1D, 0x45, 0xE1, 0x9D, 0xC5};
unsigned int keycode_map_normal[NR_SCAN_CODES*MAP_COLS] = {
    /*ScanCode      Unshift     Shift*/
    /* 0x00 */      0,          0,
    /* 0x01 */      0,          0,
    /* 0x02 */      '1',        '!',
    /* 0x03 */      '2',        '@',
    /* 0x04 */      '3',        '#',
    /* 0x05 */      '4',        '$',
    /* 0x06 */      '5',        '%',
    /* 0x07 */      '6',        '^',
    /* 0x08 */      '7',        '&',
    /* 0x09 */      '8',        '*',
    /* 0x0A */      '9',        '(',
    /* 0x0B */      '0',        ')',
    /* 0x0C */      '-',        '_',
    /* 0x0D */      '=',        '+',
    /* 0x0E */      0,          0,
    /* 0x0F */      0,          0,
    /* 0x10 */      'q',        'Q',
    /* 0x11 */      'w',        'W',
    /* 0x12 */      'e',        'E',
    /* 0x13 */      'r',        'R',
    /* 0x14 */      't',        'T',
    /* 0x15 */      'y',        'Y',
    /* 0x16 */      'u',        'U',
    /* 0x17 */      'i',        'I',
    /* 0x18 */      'o',        'O',
    /* 0x19 */      'p',        'P',
    /* 0x1A */      '[',        '{',
    /* 0x1B */      ']',        '}',
    /* 0x1C */      0,          0,      // enter
    /* 0x1D */      0x1d,       0x1d,   // ctrl left
    /* 0x1E */      'a',        'A',
    /* 0x1F */      's',        'S',
    /* 0x20 */      'd',        'D',
    /* 0x21 */      'f',        'F',
    /* 0x22 */      'g',        'G',
    /* 0x23 */      'h',        'H',
    /* 0x24 */      'j',        'J',
    /* 0x25 */      'k',        'K',
    /* 0x26 */      'l',        'L',
    /* 0x27 */      ';',        ':',
    /* 0x28 */      '\'',        '"',
    /* 0x29 */      '`',        '~',
    /* 0x2A */      0x2a,       0x2a,   // shift left
    /* 0x2B */      '\\',        '|',
    /* 0x2C */      'z',        'Z',
    /* 0x2D */      'x',        'X',
    /* 0x2E */      'c',        'C',
    /* 0x2F */      'v',        'V',
    /* 0x30 */      'b',        'B',
    /* 0x31 */      'n',        'V',
    /* 0x32 */      'm',        'V',
    /* 0x33 */      ',',        'V',
    /* 0x34 */      '.',        'V',
    /* 0x35 */      '/',        'V',
    /* 0x36 */      0x36,       0x36,   // shirt right
    /* 0x37 */      '*',        '*',
    /* 0x38 */      0x38,       0x38,   // alt left
    /* 0x39 */      ' ',        ' ',    // space
    /* 0x3A */      0,          0,      // caps lock
    /* 0x3B */      0,          0,      // f1
    /* 0x3C */      0,          0,      // f2
    /* 0x3D */      0,          0,      // f3
    /* 0x3E */      0,          0,      // f4
    /* 0x3F */      0,          0,      // f5
    0
};

void keyboard_handler(unsigned long nr, unsigned long regs);

#endif