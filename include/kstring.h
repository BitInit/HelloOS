#ifndef _BOOT_STRING_H
#define _BOOT_STRING_H

#include "types.h"

/**
 * 字符串复制，返回复制数量
 */
uint32_t strcpy(char *dst, char *src);

char* strncpy(char *dst, char *src, uint32_t n);

uint8_t strcmp(const char *str1, const char *str2);

void* memset(void *p, uint8_t val, uint_t n);

#endif