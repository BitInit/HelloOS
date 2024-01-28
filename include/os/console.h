#ifndef _OS_CONSOLE_H
#define _OS_CONSOLE_H

struct console {
    char name[16];
    void	(*write)(struct console *, const char *, unsigned);
};

#endif