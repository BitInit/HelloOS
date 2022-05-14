#include "kstring.h"

uint32 strcpy(char *dst, char *src) {
    uint32 num = 0;
    while (*src) {
        *(dst++) = *(src++);
        num++;
    }
    return num;
}

char* strncpy(char *dst, char *src, uint32 n) {
    char *p = dst;
    while (n-- > 0) {
        *(p++) = *(src++);
    }
    return dst;
}

uint8 strcmp(const char *str1, const char *str2) {
    while (*str1 && *str2 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *str2 - *str1;
}