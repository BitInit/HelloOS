#ifndef _LDR_H
#define _LDR_H

#include "ldrimg.h"

typedef struct s_img {
    ldrimg_t *ldrimg;
    fhdsc_t *fhdsc;
    char *content;
} img_t;

// 构建
// header_name grub头文件名
// ... 多个bin文件名
img_t* build(char *header_name, int file_num, char **files_name);

// 输出到文件
// out 输出文件名
// ldrimg
int output(char *out, img_t *ldrimg);

#endif
