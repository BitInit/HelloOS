#include "printk.h"
#include "kstring.h"

#define MAX_ROW 25
#define MAX_COLUMN 80

typedef struct s_cursor {
    uint32 x;
    uint32 y;
} cursor_t;

__attribute__((section(".data"))) cursor_t cursor;

void clear_screen() {
    uint16 *p = (uint16*)VGA_BASE;
    for (int32 i = 0; i <= MAX_ROW*MAX_COLUMN; i++) {
        p[i] = 0;
    }

    cursor.x = 0;
    cursor.y = 0;
    return;
}

char* numberk(char *buf, int32 n, int32 base) {
    if (n < 0 && base != 16) {
        *(buf++) = '-';
        n = -n;
    }
    
    register char *p;
    char strbuf[36];
    p = &strbuf[36];
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

void print(uint16 color, char *buf) {
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
                uint32 n = (MAX_ROW-1) * MAX_COLUMN * 2;
                char *dst = (char*)(VGA_BASE), 
                     *src = (char*)(VGA_BASE + MAX_COLUMN*2);
                strncpy(dst, src, n);
                cursor.x = 0;
                cursor.y = MAX_ROW - 1;
            }
            uint16 *p = (uint16*)(VGA_BASE + cursor.y*MAX_COLUMN*2 + cursor.x*2);
            *p = (uint16)(color << 8) + ch;
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
                uint32 n = strcpy(buf, val);
                buf += n;
            } else if (ch == 'd') {
                int32 val = va_arg(args, int32);
                buf = numberk(buf, val, 10);
            } else if (ch == 'x') {
                int32 val = va_arg(args, uint32);
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

void _printf(uint16 color, const char *fmt, va_list args) {
    char buf[512] = {0};
    vsprintf(buf, fmt, args);
    print(color, buf);
}

void printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    _printf(WORD_WHITE, fmt, args);
}

void err_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    _printf(WORD_RED, fmt, args);
}

void color_printf(uint8 color, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    _printf(color, fmt, args);
}

int main(int argc, char *argv[]) {
    printf("magic:%x\n", 0x2022babe);
    printf("sizeof:%d\n", sizeof(char*));
    return 0;
}