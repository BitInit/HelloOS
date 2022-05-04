#include "lib.h"
#include "printk.h"

void Start_Kernel(void) {
	Pos.XResolution = 1440;
	Pos.YResolution = 900;

	Pos.XPosition = 0;
	Pos.YPosition = 0;

	Pos.XCharSize = 8;
	Pos.YCharSize = 16;

	Pos.FB_addr = (int *)0xffff800000a00000;
	Pos.FB_length = (Pos.XResolution * Pos.YResolution * 4);

	for (int i = 0; i < 56; i++)
		color_printk(WHITE, BLACK, "Hello World! %d \n", i);

    while (1)
        ;
}