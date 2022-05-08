#include "svimg.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

uint8 build_header(char *header_name, img_t *img);
uint8 read_bins(int bin_num, char **bins, img_t *img);

ldrimg_t *init_ldrimg() {
    ldrimg_t *img = (ldrimg_t *)malloc(sizeof(ldrimg_t));
    img->img_magic = LDRIMG_MAGIC;
    img->fhd_size = 0;
    img->fhd_num = 0;
    return img;
}

img_t* build(char *header_name, int bin_size, char **bins) {
    img_t *img = (img_t*) malloc(sizeof(img_t));
    img->ldrimg = init_ldrimg();
    if (build_header(header_name, img))
    {
        printf("解析 grub 头文件失败\n");
        return NULL;
    }
    
    if (read_bins(bin_size, bins, img))
    {
        printf("解析 bin 文件失败\n");
        return NULL;
    }
    return img;
}

int output(char *out, img_t *img) {
    // 计算 ldrimg 占多少字节
    size_t ldrimg_size = sizeof(*(img->ldrimg));
    // 计算文件描述符总共占多少字节
    size_t fhdsc_size = sizeof((*(img->fhdsc)))*(img->ldrimg->fhd_num);

    for (int i = 0; i < img->ldrimg->fhd_num; i++)
    {
        img->fhdsc[i].fhd_posoffset += ldrimg_size + fhdsc_size;
        img->fhdsc[i].fhd_posend += ldrimg_size + fhdsc_size;
    }

    FILE *file;
    if ((file = fopen(out, "w+")) == NULL)
    {
        return -1;
    }

    // 写入镜像头
    char *val = (char *)img->ldrimg;
    fwrite(val, sizeof(char), ldrimg_size, file);
    // 写入文件描述符
    val = (char *)img->fhdsc;
    fwrite(val, sizeof(char), fhdsc_size, file);
    // 写入数据内容
    val = img->content;
    fwrite(val, sizeof(char), img->ldrimg->fhd_size, file);

    fflush(file);
    fclose(file);
    return 0;
}

uint8 build_header(char *header_name, img_t *img) {
    int fd;
    if ((fd = open(header_name, O_RDONLY)) == -1) 
    {
        return -1;
    }

    ldrimg_t *ldrimg = img->ldrimg;
    char *dst = ldrimg->header;
    char buf[BUFFER_SIZE];
    int size;
    while ((size = read(fd, buf, BUFFER_SIZE)) > 0)
    {
        memcpy(dst, buf, size);
        dst += size;
    }
    close(fd);
    return 0;
}

void close_files(FILE** files, int size) {
    for (int i = 0; i < size; i++)
    {
        fclose(files[i]);
    }
}

char *get_file_name(char *filepath) {
    char *p;
    return (p = strrchr(filepath, '/'))? p+1: filepath;
}

uint8 read_bins(int bin_num, char **bins, img_t *img) {
    ldrimg_t *ldrimg = img->ldrimg;
    ldrimg->fhd_num = bin_num;
    FILE *files[bin_num];
    for (int i = 0; i < bin_num; i++) {
        FILE *file;
        if ((file = fopen(bins[i], "r")) == NULL) {
            close_files(files, i-1);
            return -1;
        }
        files[i] = file;
    }

    fhdsc_t *fhdscs = (fhdsc_t*) malloc(bin_num * sizeof(fhdsc_t));
    for (int i = 0; i < bin_num; i++) {
        struct stat s;
        if (stat(bins[i], &s) == -1) {
            close_files(files, bin_num);
            return -1;
        }
        fhdscs[i].fhd_size = s.st_size;
        fhdscs[i].fhd_posoffset = ldrimg->fhd_size;
        ldrimg->fhd_size += s.st_size;
        fhdscs[i].fhd_posend = ldrimg->fhd_size;

        strcpy(fhdscs[i].fhd_name, get_file_name(bins[i]));
    }
    img->fhdsc = fhdscs;
    img->content = (char*) malloc(sizeof(char)*ldrimg->fhd_size);

    char buf[BUFFER_SIZE];
    char *p = img->content;
    for (int i = 0; i < bin_num; i++) {
        size_t size;
        while ((size = fread(buf, sizeof(char), BUFFER_SIZE,files[i])) > 0) {
            memcpy(p, buf, size);
            p += size;
        }
    }
    close_files(files, bin_num);
    return 0;
}