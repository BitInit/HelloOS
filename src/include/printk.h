/**
 * 保护模式下屏幕显示
 */
#ifndef _HELLOOS_PRINTK_H
#define _HELLOOS_PRINTK_H

#include "types.h"

#define VGA_BASE 0xb8000
#define VGA_END  0xbffff

#define WORD_RED 0x04
#define WORD_WHITE 0x07
#define WORD_BROWN  0x06
#define WORD_YELLOW 0x0E

/**
 * 清屏
 */
void clear_screen();

void kprintf(const char *fmt, ...);

/**
 * 打印错误日志
 */
void kerr_printf(const char *fmt, ...);

/**
 * 带有颜色的信息打印
 * color：高4位背景色，低4位前景色
 */
void kcolor_printf(uint8_t color, const char *fmt, ...);

void kerrorf(const char *fmt, ...);

#endif