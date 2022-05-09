/**
 * 保护模式下屏幕显示
 */
#ifndef _PRINTK_H
#define _PRINTK_H

#include "ktype.h"
#include <stdarg.h>

#define VGA_BASE 0xb8000
#define VGA_END  0xbffff

#define WORD_RED 0x04
#define WORD_WHITE 0x07

/**
 * 清屏
 */
void clear_screen();

void printf(const char *fmt, ...);

/**
 * 打印错误日志
 */
void err_printf(const char *fmt, ...);

/**
 * 带有颜色的信息打印
 * color：高4位背景色，低4位前景色
 */
void color_printf(uint8 color, const char *fmt, ...);

#endif