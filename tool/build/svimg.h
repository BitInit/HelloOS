#ifndef _LDR_H
#define _LDR_H

#include "ldrimg.h"

// 构建
// grub_header grub头文件名
// ... 多个bin文件名
ldrimg_t* build(char *grub_header, int bin_size, char **bins);

// 输出到文件
// out 输出文件名
// ldrimg
uint8 output(char *out, ldrimg_t *ldrimg);

#endif
