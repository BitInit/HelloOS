#include "svimg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIN_MAX_SIZE 30

typedef struct s_opt
{
    char *output;
    char *grub_header;
    int bin_size;
    char *bins[BIN_MAX_SIZE];
} opt_t;

void print_usage();
opt_t* parse_arg(int argc, char* argv[]);
void free_img(img_t *img);

/**
 * bimg -o build.img -g grub_header_bin_file -b bin1.img bin2.img bin3.img...
 */
int main(int argc, char* argv[]) 
{
    if (argc <= 1)
    {
        print_usage("参数错误");
        return -1;
    }
    
    opt_t *opt = parse_arg(argc, argv);
    if (!opt)
    {
        print_usage("参数错误");
        return -1;
    }

    img_t *img = build(opt->grub_header, opt->bin_size, opt->bins);
    if (!img)
    {
        return -1;
    }

    uint8 ret = output(opt->output, img);
    if (!ret)
    {
        free(opt);
        free_img(img);
        return -1;
    }

    free(opt);
    free_img(img);
    return 0;
}

void print_usage(const char *msg)
{
    printf("%s\n\nUsage:\n", msg);
    printf("\tbimg -o build.img -g grub_header_bin_file -b bin1.img bin2.img bin3.img...\n");
    printf("\t-o 输出文件\n");
    printf("\t-g grub头文件\n");
    printf("\t-b 多个bin文件\n");
}

opt_t* parse_arg(int argc, char* argv[])
{
    opt_t *opt = (opt_t*)malloc(sizeof(opt_t));
    opt->bin_size = 0;
    char opt_name;
    for (int i = 1; i < argc; i++) 
    {
        if (!strcmp("-o", argv[i]) || !strcmp("-g", argv[i]) || !strcmp("-b", argv[i])) {
            opt_name = argv[i][1];
            continue;
        }
        
        switch (opt_name)
        {
        case 'o':
            opt->output = argv[i];
            break;
        case 'g':
            opt->grub_header = argv[i];
            break;
        case 'b':
            opt->bins[opt->bin_size++] = argv[i];
            break;
        default:
            break;
        }
    }

    if (!opt->bin_size || !opt->output || !opt->grub_header)
    {
        return NULL;
    }
    
    return opt;
}

void free_img(img_t *img)
{
    if (img->ldrimg)
    {
        free(img->ldrimg);
    }
    if (img->fhdsc)
    {
        free(img->fhdsc);
    }
    if (img->content)
    {
        free(img->content);
    }
}