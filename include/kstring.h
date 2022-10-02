#ifndef _BOOT_STRING_H
#define _BOOT_STRING_H

#include "types.h"

/**
 * 字符串复制，返回复制数量
 */
uint32_t strcpy(char *dst, char *src);

int strlen(char *String);

char* strncpy(char *dst, char *src, uint32_t n);

uint8_t strcmp(const char *str1, const char *str2);

void* memset(void *p, uint8_t val, uint_t n);

static inline void * memcpy(void *From,void * To,long Num)
{
	int d0,d1,d2;
	__asm__ __volatile__	(	"cld	\n\t"
        "rep	\n\t"
        "movsq	\n\t"
        "testb	$4,%b4	\n\t"
        "je	1f	\n\t"
        "movsl	\n\t"
        "1:\ttestb	$2,%b4	\n\t"
        "je	2f	\n\t"
        "movsw	\n\t"
        "2:\ttestb	$1,%b4	\n\t"
        "je	3f	\n\t"
        "movsb	\n\t"
        "3:	\n\t"
        :"=&c"(d0),"=&D"(d1),"=&S"(d2)
        :"0"(Num/8),"q"(Num),"1"(To),"2"(From)
        :"memory"
    );
	return To;
}

#endif