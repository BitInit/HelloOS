#ifndef _STRING_H
#define _STRING_H

#include "type.h"

/**
 * 字符串复制，返回复制数量
 */
size_t strcpy(char *dst, char *src);

char* strncpy(char *dst, char *src, size_t n);

#endif