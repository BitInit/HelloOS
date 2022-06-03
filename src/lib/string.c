#include "string.h"

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