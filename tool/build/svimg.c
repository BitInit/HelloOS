#include "svimg.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

uint8 read_grub(char *grub_header, ldrimg_t *ldrimg);
uint8 read_bins(int bin_size, char **bins, ldrimg_t *ldrimg);

ldrimg_t* build(char *grub_header, int bin_size, char **bins) 
{
    ldrimg_t *ldrimg = (ldrimg_t *)malloc(sizeof(ldrimg_t));
    if (read_grub(grub_header, ldrimg))
    {
        printf("解析 grub 头文件失败\n");
        return NULL;
    }
    
    if (read_bins(bin_size, bins, ldrimg))
    {
        printf("解析 bin 文件失败\n");
        return NULL;
    }
    return ldrimg;
}

uint8 output(char *out, ldrimg_t *ldrimg)
{
    printf("output to file\n");
    return 0;
}

uint8 read_grub(char *grub_header, ldrimg_t *ldrimg)
{
    int fd;
    if ((fd = open(grub_header, O_RDONLY)) == -1) 
    {
        return -1;
    }

    char *dst = ldrimg->grub_header;
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

void close_files(int *fds, int size)
{
    for (int i = 0; i < size; i++)
    {
        close(fds[i]);
    }
}

char *get_file_name(char *filepath)
{
    char *p;
    return (p = strrchr(filepath, '/'))? p+1: filepath;
}

uint8 read_bins(int bin_size, char **bins, ldrimg_t *ldrimg)
{
    ldrimg->fhd_num = bin_size;
    int bin_fds[bin_size];
    for (int i = 0; i < bin_size; i++)
    {
        int fd;
        if ((fd = open(bins[i], O_RDONLY)) == -1)
        {
            close_files(bin_fds, i-1);
            return -1;
        }
        bin_fds[i] = fd;
    }

    fhdsc_t *fhdscs = (fhdsc_t*) malloc(bin_size*sizeof(fhdsc_t));
    for (int i = 0; i < bin_size; i++)
    {
        struct stat s;
        if (stat(bins[i], &s) == -1)
        {
            close_files(bin_fds, bin_size);
            return -1;
        }
        fhdscs[i].fhd_size = s.st_size;
        ldrimg->fhd_size += s.st_size;

        strcpy(fhdscs[i].fhd_name, get_file_name(bins[i]));
    }
    ldrimg->fhdscs = fhdscs;
    ldrimg->f_content = (char*) malloc(sizeof(char)*ldrimg->fhd_size);

    char buf[BUFFER_SIZE];
    char *p = ldrimg->f_content;
    for (int i = 0; i < bin_size; i++)
    {
        int size;
        while ((size = read(bin_fds[i], buf, O_RDONLY)) > 0)
        {
            memcpy(p, buf, size);
            p += size;
        }
    }
    close_files(bin_fds, bin_size);
    return 0;
}