/**
 * 实模式或保护模式下屏幕显示
 */
#ifndef _PRINTS_H
#define _PRINTS_H

#include "type.h"
#include <stdarg.h>

#define VGA_BASE 0xb8000
#define VGA_END  0xbffff

// 清屏
void clear_screen();

void prints(char *str, int32 num);

void vsprintf(char *buf, const char *fmt, va_list args);

#endif