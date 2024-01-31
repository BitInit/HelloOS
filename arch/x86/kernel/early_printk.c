#include <stdarg.h>

#include <asm/io.h>
#include <asm/page.h>

#include <os/console.h>
#include <os/kernel.h>

#define VGABASE		(PAGE_OFFSET + 0xb8000)
static long max_ypos = 25, max_xpos = 80;
static long current_ypos = 25, current_xpos;

void early_vga_write(struct console *con, const char *str, unsigned n) {
    char c;
	int  i, k, j;

	while ((c = *str++) != '\0' && n-- > 0) {
		// 向上滚动一行
		if (current_ypos >= max_ypos) {
			for (k = 1, j = 0; k < max_ypos; k++, j++) {
				for (i = 0; i < max_xpos; i++) {
					writew(readw((void*)(VGABASE+2*(max_xpos*k+i))), (void*)(VGABASE + 2*(max_xpos*j + i)));
				}
			}
			// 将最后一行设置空
			for (i = 0; i < max_xpos; i++)
				writew(0x720, (void*)(VGABASE + 2*(max_xpos*j + i)));
			current_ypos = max_ypos-1;
		}
		if (c == '\n') {
			current_xpos = 0;
			current_ypos++;
		} else if (c != '\r')  {
			writew(((0x7 << 8) | (unsigned short) c), (void*)(VGABASE + 2*(max_xpos*current_ypos + current_xpos++)));
			if (current_xpos >= max_xpos) {
				current_xpos = 0;
				current_ypos++;
			}
		}
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