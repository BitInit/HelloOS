#include "mm.h"
#include "kprint.h"

mm_page_t* alloc_pages(int zone_select, int n, uint64_t flags) {
	int i;
	unsigned long page = 0;

	int zone_start = 0;
	int zone_end = 0;

	switch(zone_select)
	{
		case ZONE_DMA:
				zone_start = 0;
				zone_end = ZONE_DMA_INDEX;
			break;

		case ZONE_NORMAL:
				zone_start = ZONE_DMA_INDEX;
				zone_end = ZONE_NORMAL_INDEX;
			break;

		case ZONE_UNMAPED:
				zone_start = ZONE_UNMAPED_INDEX;
				zone_end = gmdsc.zones_size - 1;
			break;

		default:
			kerror("alloc_pages error zone_select index\n");
			return NULL;
			break;
	}

	for(i = zone_start;i <= zone_end; i++)
	{
		mm_zone_t *z;
		unsigned long j;
		unsigned long start,end,length;
		unsigned long tmp;

		if((gmdsc.zones_struct + i)->page_free_count < n)
			continue;

		z = gmdsc.zones_struct + i;
		start = z->zone_start_addr >> PAGE_4K_SHIFT;
		end = z->zone_end_addr >> PAGE_4K_SHIFT;
		length = z->zone_length >> PAGE_4K_SHIFT;

		tmp = 64 - start % 64;

		for(j = start;j <= end;j += j % 64 ? tmp : 64)
		{
			unsigned long * p = gmdsc.page_bits + (j >> 6);
			unsigned long shift = j % 64;
			unsigned long k;
			for(k = shift;k < 64 - shift;k++)
			{
				if( !(((*p >> k) | (*(p + 1) << (64 - k))) & (n == 64 ? 0xffffffffffffffffUL : ((1UL << n) - 1))) )
				{
					unsigned long l;
					page = j + k - 1;
					for(l = 0;l < n;l++)
					{
						mm_page_t *x = gmdsc.pages_struct + page + l;
						page_init(x, flags);
					}
					goto find_free_pages;
				}
			}
		
		}
	}

	return NULL;

find_free_pages:

	return (mm_page_t *)(gmdsc.pages_struct + page);
}