#ifndef _HELLOOS_KKPRINT
#define _HELLOOS_KKPRINT

#include "printk.h"

#define kinfo(...) {            \
        kprintf("[info] ");     \
        kprintf(__VA_ARGS__);   \
    }

#define kdebug(...) {                                                      \
        kcolor_printf(WORD_BROWN, "[debug] (%s:%d) ", __FILE__, __LINE__); \
        kcolor_printf(WORD_BROWN, __VA_ARGS__);                            \
    }

#define kwarn(...) {                                \
        kcolor_printf(WORD_YELLOW, "[warn] ");      \
        kcolor_printf(WORD_YELLOW, __VA_ARGS__);    \
    }

#define kerror(...) {               \
        kerr_printf("[error] ");    \
        kerr_printf(__VA_ARGS__);   \
    }

#endif