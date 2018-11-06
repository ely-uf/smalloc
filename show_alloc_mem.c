#include "malloc_internal.h"

# include <stdio.h>

size_t	get_total_regions(t_malloc_zone *zone)
{
	size_t		res;
	t_region	*region_iter;

	res = 0;
	region_iter = zone->regions;
	while (region_iter)
	{
		region_iter = region_iter->next;
		res++;
	}
	return (res);
}

void	print_zones(t_malloc_zone *zone, char *zone_type)
{
	t_region		*region;
	char			*region_mem;

	while (zone)
	{
		printf("%s: %p\n", zone_type, zone);
		printf("\tmin_regions -> %zu\n\tmax_region_size -> %zu\n",
				zone->num_regions, zone->region_size);
		printf("\ttotal_regions -> %zu\n", get_total_regions(zone));
		region = zone->regions;
		while (region)
		{
			if (!region->free)
			{
				region_mem = (char*)(region + 1);
				printf("%p - %p : %zu %s\n", region_mem,
						region_mem + region->size, region->size,
						region->size == 1 ? "byte" : "bytes");
			}
			region = region->next;
		}
		zone = zone->next_zone;
	}
}

void	show_alloc_mem(void)
{
	t_malloc_mdata *mdata;

	mdata = get_malloc_mdata();
	print_zones(mdata->tiny_zones, "TINY");
	print_zones(mdata->small_zones, "SMALL");
	print_zones(mdata->large_zones, "LARGE");
}
