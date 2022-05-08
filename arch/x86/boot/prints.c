#include "prints.h"

typedef struct s_cursor {
    uint32 pos;
} cursor_t;

__attribute__((section(".data"))) cursor_t cursor;

void clear_screen() {
    uint16 *p = (uint16*)VGA_BASE;
    for (int32 i = 0; i < 2001; i++) {
        p[i] = 0;
    }

    cursor.pos = VGA_BASE;
    return;
}

void prints(char *str, int32 num) {
    uint16 *p = (uint16*) cursor.pos;
    for (int32 i = 0; i < num; i++) {
        p[i] = (uint16)0x0700 + (uint16)str[i];
    }
    cursor.pos += num*2;
    return;
}

void vsprintf(char *buf, const char *fmt, va_list args) {

    return;
}