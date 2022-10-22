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

int skip_atoi(const char **s) {
	int i=0;

	while (is_digit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
}

static char * number(char * str, long num, int base, int size, int precision,	int type) {
	char c,sign,tmp[50];
	const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i;

	if (type&SMALL) digits = "0123456789abcdefghijklmnopqrstuvwxyz";
	if (type&LEFT) type &= ~ZEROPAD;
	if (base < 2 || base > 36)
		return 0;
	c = (type & ZEROPAD) ? '0' : ' ' ;
	sign = 0;
	if (type&SIGN && num < 0) {
		sign='-';
		num = -num;
	} else
		sign=(type & PLUS) ? '+' : ((type & SPACE) ? ' ' : 0);
	if (sign) size--;
	if (type & SPECIAL)
		if (base == 16) size -= 2;
		else if (base == 8) size--;
	i = 0;
	if (num == 0)
		tmp[i++]='0';
	else while (num!=0)
		tmp[i++]=digits[do_div(num,base)];
	if (i > precision) precision=i;
	size -= precision;
	if (!(type & (ZEROPAD + LEFT)))
		while(size-- > 0)
			*str++ = ' ';
	if (sign)
		*str++ = sign;
	if (type & SPECIAL)
		if (base == 8)
			*str++ = '0';
		else if (base==16) 
		{
			*str++ = '0';
			*str++ = digits[33];
		}
	if (!(type & LEFT))
		while(size-- > 0)
			*str++ = c;

	while(i < precision--)
		*str++ = '0';
	while(i-- > 0)
		*str++ = tmp[i];
	while(size-- > 0)
		*str++ = ' ';
	return str;
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
    while (*p != 0) {
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
				memset((char*)(VGA_BASE) + (MAX_ROW - 1)*MAX_COLUMN*2, 0, MAX_COLUMN*2);
            }
            uint16_t *p = (uint16_t*)((uint64_t)(VGA_BASE + cursor.y*MAX_COLUMN*2 + cursor.x*2));
            *p = (uint16_t)(color << 8) + ch;
            cursor.x++;
        }
    }
    return;
}

void vsprintf(char *buf, const char *fmt, va_list args) {
    // char ch;
    // while (ch = *(fmt++)) {
    //     if (ch == '%') {
    //         ch = *(fmt++);
    //         if (ch == 's') {
    //             char *val = va_arg(args, char*);
    //             uint64_t n = strcpy(buf, val);
    //             buf += n;
    //         } else if (ch == 'd') {
    //             int64_t val = va_arg(args, int64_t);
    //             buf = numberk(buf, val, 10);
    //         } else if (ch == 'x') {
    //             uint64_t val = va_arg(args, uint64_t);
    //             *(buf++) = '0';
    //             *(buf++) = 'x';
    //             buf = numberk(buf, val, 16);
    //         }
    //     } else {
    //         *(buf++) = ch;
    //     }
    // }    
    char * str,*s;
	int flags;
	int field_width;
	int precision;
	int len,i;

	int qualifier;		/* 'h', 'l', 'L' or 'Z' for integer fields */

	for(str = buf; *fmt; fmt++)
	{

		if(*fmt != '%')
		{
			*str++ = *fmt;
			continue;
		}
		flags = 0;
		repeat:
			fmt++;
			switch(*fmt)
			{
				case '-':flags |= LEFT;	
				goto repeat;
				case '+':flags |= PLUS;	
				goto repeat;
				case ' ':flags |= SPACE;	
				goto repeat;
				case '#':flags |= SPECIAL;	
				goto repeat;
				case '0':flags |= ZEROPAD;	
				goto repeat;
			}

			/* get field width */

			field_width = -1;
			if(is_digit(*fmt))
				field_width = skip_atoi(&fmt);
			else if(*fmt == '*')
			{
				fmt++;
				field_width = va_arg(args, int);
				if(field_width < 0)
				{
					field_width = -field_width;
					flags |= LEFT;
				}
			}
			
			/* get the precision */

			precision = -1;
			if(*fmt == '.')
			{
				fmt++;
				if(is_digit(*fmt))
					precision = skip_atoi(&fmt);
				else if(*fmt == '*')
				{	
					fmt++;
					precision = va_arg(args, int);
				}
				if(precision < 0)
					precision = 0;
			}
			
			qualifier = -1;
			if(*fmt == 'h' || *fmt == 'l' || *fmt == 'L' || *fmt == 'Z')
			{	
				qualifier = *fmt;
				fmt++;
			}
							
			switch(*fmt)
			{
				case 'c':

					if(!(flags & LEFT))
						while(--field_width > 0)
							*str++ = ' ';
					*str++ = (unsigned char)va_arg(args, int);
					while(--field_width > 0)
						*str++ = ' ';
					break;

				case 's':
				
					s = va_arg(args,char *);
					if(!s)
						s = '\0';
					len = strlen(s);
					if(precision < 0)
						precision = len;
					else if(len > precision)
						len = precision;
					
					if(!(flags & LEFT))
						while(len < field_width--)
							*str++ = ' ';
					for(i = 0;i < len ;i++)
						*str++ = *s++;
					while(len < field_width--)
						*str++ = ' ';
					break;

				case 'o':
					
					if(qualifier == 'l')
						str = number(str,va_arg(args,unsigned long),8,field_width,precision,flags);
					else
						str = number(str,va_arg(args,unsigned int),8,field_width,precision,flags);
					break;

				case 'p':

					if(field_width == -1)
					{
						field_width = 2 * sizeof(void *);
						flags |= ZEROPAD;
					}

					str = number(str,(unsigned long)va_arg(args,void *),16,field_width,precision,flags);
					break;

				case 'x':

					flags |= SMALL;

				case 'X':

					if(qualifier == 'l')
						str = number(str,va_arg(args,unsigned long),16,field_width,precision,flags);
					else
						str = number(str,va_arg(args,unsigned int),16,field_width,precision,flags);
					break;

				case 'd':
				case 'i':

					flags |= SIGN;
				case 'u':

					if(qualifier == 'l')
						str = number(str,va_arg(args,unsigned long),10,field_width,precision,flags);
					else
						str = number(str,va_arg(args,unsigned int),10,field_width,precision,flags);
					break;

				case 'n':
					
					if(qualifier == 'l')
					{
						long *ip = va_arg(args,long *);
						*ip = (str - buf);
					}
					else
					{
						int *ip = va_arg(args,int *);
						*ip = (str - buf);
					}
					break;

				case '%':
					
					*str++ = '%';
					break;

				default:

					*str++ = '%';	
					if(*fmt)
						*str++ = *fmt;
					else
						fmt--;
					break;
			}

	}
	*str = '\0';
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

void kerrorf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    _printf(WORD_RED, fmt, args);
    va_end(args);
    while (1) ;
    return;
}