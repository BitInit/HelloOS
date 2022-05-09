#include "kstring.h"

size_t strcpy(char *dst, char *src) {
    size_t num = 0;
    while (*src) {
        *(dst++) = *(src++);
        num++;
    }
    return num;
}

char* strncpy(char *dst, char *src, size_t n) {
    char *p = dst;
    while (n-- > 0) {
        *(p++) = *(src++);
    }
    return dst;
}