#ifndef _LDR_H
#define _LDR_H

#include "ldrimg.h"

typedef struct s_img
{
    ldrimg_t *ldrimg;
    fhdsc_t *fhdsc;
    char *content;
} img_t;

// 构建
// grub_header grub头文件名
// ... 多个bin文件名
img_t* build(char *grub_header, int bin_size, char **bins);

// 输出到文件
// out 输出文件名
// ldrimg
uint8 output(char *out, img_t *ldrimg);

#endif
