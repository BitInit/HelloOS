#include <os/string.h>

#ifndef __HAVE_ARCH_MEMSET
void *memset(void *s, int c, size_t count) {
	char *xs = s;

	while (count--)
		*xs++ = c;
	return s;
}
#endif

#ifndef __HAVE_ARCH_STRNLEN
__kernel_size_t strnlen(const char *s, __kernel_size_t count) {
	const char *sc;

	for (sc = s; count-- && *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}
#endif