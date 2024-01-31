#ifndef _OS_CTYPE_H
#define _OS_CTYPE_H

static inline int isdigit(char c) {
    return c >= '0' && c <= '9';
}

static inline int isalnum(char c) {
    return c >= 'a' && c <= 'z' || (c >= 'A' && c <= 'Z');
}

#endif