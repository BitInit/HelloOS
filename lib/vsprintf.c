#include <stdarg.h>
#include <os/types.h>

int vscnprintf(char *buf, size_t size, const char *fmt, va_list args) {
    int i = 0;
    for (; fmt[i]; i++) {
        buf[i] = fmt[i];
    }
    return i;
}
