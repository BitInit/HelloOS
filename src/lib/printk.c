#include "printk.h"
#include "kstring.h"
#include <stdarg.h>

#define MAX_ROW 25
#define MAX_COLUMN 80

typedef struct s_cursor {
    uint32_t x;
    uint32_t y;
} cursor_t;

__attribute__((section(".data"))) cursor_t cursor;

void clear_screen() {
    uint16_t *p = (uint16_t*)VGA_BASE;
    for (int32_t i = 0; i <= MAX_ROW*MAX_COLUMN; i++) {
        p[i] = 0;
    }

    cursor.x = 0;
    cursor.y = 0;
    return;
}

char* numberk(char *buf, int64_t n, int32_t base) {
    if (n < 0 && base != 16) {
        *(buf++) = '-';
        n = -n;
    }
    
    register char *p;
    char strbuf[64];
    p = &strbuf[64];
    *--p = 0;

    if (n == 0) {
        *--p = '0';
    } else {
        do {
            *--p = "0123456789abcdef"[n % base];
        } while (n /= base);
    }
    while (*p != 0)
    {
        *buf++ = *p++;
    }
    return buf;
}

void print(uint16_t color, char *buf) {
    char ch;
    while (ch = *(buf++)) {
        if (ch == '\n') {
            cursor.x = 0;
            cursor.y++;
        } else if (ch == '\t') {
            cursor.x += 4;
        } else {
            if (cursor.x >= MAX_COLUMN-1) {
                cursor.x = 0;
                cursor.y++;
            }
            if (cursor.y >= MAX_ROW) {
                // 25 行屏幕全部占完，滚动显示日志
                uint32_t n = (MAX_ROW-1) * MAX_COLUMN * 2;
                char *dst = (char*)(VGA_BASE), 
                     *src = (char*)(VGA_BASE + MAX_COLUMN*2);
                strncpy(dst, src, n);
                cursor.x = 0;
                cursor.y = MAX_ROW - 1;
            }
            uint16_t *p = (uint16_t*)((uint64_t)(VGA_BASE + cursor.y*MAX_COLUMN*2 + cursor.x*2));
            *p = (uint16_t)(color << 8) + ch;
            cursor.x++;
        }
    }
    return;
}

void vsprintf(char *buf, const char *fmt, va_list args) {
    char ch;
    while (ch = *(fmt++)) {
        if (ch == '%') {
            ch = *(fmt++);
            if (ch == 's') {
                char *val = va_arg(args, char*);
                uint64_t n = strcpy(buf, val);
                buf += n;
            } else if (ch == 'd') {
                int64_t val = va_arg(args, int64_t);
                buf = numberk(buf, val, 10);
            } else if (ch == 'x') {
                uint64_t val = va_arg(args, uint64_t);
                *(buf++) = '0';
                *(buf++) = 'x';
                buf = numberk(buf, val, 16);
            }
        } else {
            *(buf++) = ch;
        }
    }    
    return;
}

void _printf(uint16_t color, const char *fmt, va_list args) {
    char buf[512] = {0};
    vsprintf(buf, fmt, args);
    print(color, buf);
}

void kprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    _printf(WORD_WHITE, fmt, args);
    va_end(args);
}

void kerr_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    _printf(WORD_RED, fmt, args);
    va_end(args);
}

void kcolor_printf(uint8_t color, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    _printf(color, fmt, args);
    va_end(args);
}

void kerror(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    _printf(WORD_RED, fmt, args);
    va_end(args);
    while (1) ;
    return;
}