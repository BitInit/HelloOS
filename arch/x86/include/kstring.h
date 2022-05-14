#ifndef _KSTRING_H
#define _KSTRING_H

#include "ktype.h"

/**
 * 字符串复制，返回复制数量
 */
uint32 strcpy(char *dst, char *src);

char* strncpy(char *dst, char *src, uint32 n);

uint8 strcmp(const char *str1, const char *str2);

#endif