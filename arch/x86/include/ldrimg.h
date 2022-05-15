#ifndef _LDRIMG_H
#define _LDRIMG_H

#include "ktype.h"

#define LDRIMG_MAGIC 0x2022babe
#define HEADER_SIZE 4096
#define FH_MAX_NAME_LEN 192 // 文件名最大长度

#define IMG_BASE 0x8000000
#define LDRFILE_BASE (IMG_BASE + sizeof(ldrimg_t))

#define MACHINE_CHECK_BIN "machineinfocheck.bin"
#define MACHINE_CHECK_ADDR 0x400000

#define KERNEL_BIN "kernel.bin"
#define KERNEL_ADDR 0x800000

typedef struct s_fhdsc {
    uint32 fhd_posoffset;               // 文件开始偏移位置
    uint32 fhd_posend;                  // 文件位置偏移结束
    uint32 fhd_size;                    // 文件大小
    uint32 fhd_sum;                     // 文件校验和
    char   fhd_name[FH_MAX_NAME_LEN];   // 文件名
} fhdsc_t;

/**
 * 文件格式：
 * 
 * 4KB 头文件（固定）
 * 文件1描述符
 * 文件2描述符
 * ...
 * 文件1
 * 文件2
 * ...
 */
typedef struct s_ldrimg {
    char header[HEADER_SIZE];
    uint32 img_magic;
    uint32 fhd_num;
    uint32 fhd_size;
} ldrimg_t;

#endif