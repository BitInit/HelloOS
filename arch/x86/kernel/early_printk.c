#include <stdarg.h>

#include <asm/io.h>
#include <asm/page.h>

#include <os/console.h>
#include <os/kernel.h>

#define VGABASE		(PAGE_OFFSET + 0xb8000)

void early_vga_write(struct console *con, const char *str, unsigned n) {
    unsigned short *dst = (unsigned short*)VGABASE;
    for (int i = 0; i < 1000; i++) {
        writew((0x7 << 8) | (unsigned short) 'h', dst);
        dst++;
    }
}

static struct console early_vga_console = {
	.name =		"earlyvga",
	.write =	early_vga_write
};

static struct console *early_console = &early_vga_console;

void early_printk(const char *fmt, ...) {
    char buf[512];
	int n;
	va_list ap;

	va_start(ap, fmt);
	n = vscnprintf(buf, 512, fmt, ap);
	early_console->write(early_console, buf, n);
	va_end(ap);
}