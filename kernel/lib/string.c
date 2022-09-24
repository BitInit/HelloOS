#include "kstring.h"

uint32_t strcpy(char *dst, char *src) {
    uint32_t num = 0;
    while (*src) {
        *(dst++) = *(src++);
        num++;
    }
    return num;
}

char* strncpy(char *dst, char *src, uint32_t n) {
    char *p = dst;
    while (n-- > 0) {
        *(p++) = *(src++);
    }
    return dst;
}

uint8_t strcmp(const char *str1, const char *str2) {
    while (*str1 && *str2 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *str2 - *str1;
}

void* memset(void *p, uint8_t val, uint_t n) {
    int d0,d1;
	unsigned long tmp = val * 0x0101010101010101UL;
	__asm__	__volatile__	(	"cld	\n\t"
					"rep	\n\t"
					"stosq	\n\t"
					"testb	$4, %b3	\n\t"
					"je	1f	\n\t"
					"stosl	\n\t"
					"1:\ttestb	$2, %b3	\n\t"
					"je	2f\n\t"
					"stosw	\n\t"
					"2:\ttestb	$1, %b3	\n\t"
					"je	3f	\n\t"
					"stosb	\n\t"
					"3:	\n\t"
					:"=&c"(d0),"=&D"(d1)
					:"a"(tmp),"q"(n),"0"(n/8),"1"(p)	
					:"memory"					
				);
	return p;
}

int strlen(char *String) {
	register int __res;
	__asm__	__volatile__	(	"cld	\n\t"
		"repne	\n\t"
		"scasb	\n\t"
		"notl	%0	\n\t"
		"decl	%0	\n\t"
		:"=c"(__res)
		:"D"(String),"a"(0),"0"(0xffffffff)
		:
	);
	return __res;
}
