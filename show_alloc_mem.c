#include "malloc_internal.h"

# include <stdio.h>

void	print_zones(t_malloc_zone *zone, char *zone_type)
{
	t_region		*region;
	char			*region_mem;

	while (zone)
	{
		printf("%s: %p\n", zone_type, zone);
		printf("\tRegions -> %zu\n\tregion_size -> %zu\n",
				zone->num_regions, zone->region_size);
		region = zone->regions;
		while (region)
		{
			region_mem = (char*)(region + 1);
			printf("%#X - %#X : %u %s\n", region_mem,
					region_mem + region->size, region->size,
					region->size == 1 ? "byte" : "bytes");
			region = region->next;
		}
		zone = zone->next_zone;
	}
}

void	show_alloc_mem(void)
{
	t_malloc_mdata *mdata;
	size_t			total;

	mdata = get_malloc_mdata();
	print_zones(mdata->tiny_zones, "TINY");
	print_zones(mdata->small_zones, "SMALL");
	print_zones(mdata->large_zones, "LARGE");
}
