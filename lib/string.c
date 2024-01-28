#include <os/string.h>

#ifndef __HAVE_ARCH_MEMSET
void *memset(void *s, int c, size_t count)
{
	char *xs = s;

	while (count--)
		*xs++ = c;
	return s;
}
#endif