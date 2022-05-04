#ifndef _LDRIMG_H
#define _LDRIMG_H

#include "type.h"

#define GRUB_HEADER_SIZE 4096
#define FH_MAX_NAME_LEN 192 // 文件名最大长度

typedef struct s_fhdsc
{
    uint64 fhd_posoffset;         // 文件开始偏移位置
    uint64 fhd_posend;            // 文件位置偏移结束
    uint64 fhd_size;              // 文件大小
    uint64 fhd_sum;               // 文件校验和
    char fhd_name[FH_MAX_NAME_LEN]; // 文件名
} fhdsc_t;

/**
 * grub 加载文件，文件格式：
 * 
 * 4KB grub头文件（固定）
 * 文件1描述符
 * 文件2描述符
 * ...
 * 文件1
 * 文件2
 * ...
 */
typedef struct s_ldrimg
{
    char grub_header[GRUB_HEADER_SIZE];
    uint32 fhd_num;
    uint64 fhd_size;
    fhdsc_t *fhdscs;
    char *f_content;
} ldrimg_t;

#endif