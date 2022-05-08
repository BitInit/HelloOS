#include "svimg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIN_MAX_SIZE 30

typedef struct s_context {
    char *output;
    char *header_name;
    int file_num;
    char *files_name[BIN_MAX_SIZE];
} context_t;

void print_usage();
context_t* parse_arg(int argc, char* argv[]);
void free_img(img_t *img);

/**
 * bimg -o build.img -h <header_file> -f file1 file2 file3...
 */
int main(int argc, char* argv[]) {
    if (argc <= 1) {
        print_usage("参数错误");
        return -1;
    }
    
    context_t *context = parse_arg(argc, argv);
    if (!context) {
        print_usage("参数错误");
        return -1;
    }

    img_t *img = build(context->header_name, context->file_num, context->files_name);
    if (!img) {
        return -1;
    }

    int ret = output(context->output, img);
    free(context);
    free_img(img);
    
    if (ret) {
        printf("构建包失败\n");
        return -1;
    }
    return 0;
}

void print_usage(const char *msg) {
    printf("%s\n\nUsage:\n", msg);
    printf("\tbimg -o build.img -h header_file -f file1 file2 file3...\n");
    printf("\t-o 输出文件\n");
    printf("\t-h 头文件\n");
    printf("\t-f 文件\n");
}

context_t* parse_arg(int argc, char* argv[]) {
    context_t *context = (context_t*)malloc(sizeof(context_t));
    context->file_num = 0;
    char opt_name;
    for (int i = 1; i < argc; i++) {
        if (!strcmp("-o", argv[i]) || !strcmp("-h", argv[i]) || !strcmp("-f", argv[i])) {
            opt_name = argv[i][1];
            continue;
        }
        
        switch (opt_name) {
        case 'o':
            context->output = argv[i];
            break;
        case 'h':
            context->header_name = argv[i];
            break;
        case 'f':
            context->files_name[context->file_num++] = argv[i];
            break;
        default:
            break;
        }
    }

    if (!context->output || !context->header_name) {
        return NULL;
    }
    
    return context;
}

void free_img(img_t *img) {
    if (img->ldrimg) {
        free(img->ldrimg);
    }
    if (img->fhdsc) {
        free(img->fhdsc);
    }
    if (img->content) {
        free(img->content);
    }
}