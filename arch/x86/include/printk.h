/**
 * 保护模式下屏幕显示
 */
#ifndef _PRINTK_H
#define _PRINTK_H

#include "ktype.h"
#include <stdarg.h>

#define VGA_BASE 0xb8000
#define VGA_END  0xbffff

// 清屏
void clear_screen();

void printf(const char *fmt, ...);

#endif