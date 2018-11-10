#include "malloc_zone.h"
#include "malloc_internal.h"

t_malloc_zone	*malloc_alloc_zone(size_t num_regions, size_t region_size)
{
	static int		page_size;
	size_t			min_size;
	size_t			actual_size;
	t_malloc_zone	*new_zone;

	if (!page_size)
		page_size = getpagesize();
	min_size = num_regions * (region_size + sizeof(t_region)) +
		sizeof(t_malloc_zone);
	actual_size = page_size;
	actual_size = ROUND_TO(min_size, page_size);
	new_zone = mmap(NULL, actual_size, PROT_ALL, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (new_zone == MAP_FAILED)
		return (NULL);
	new_zone->regions = (t_region *)(new_zone + 1);
	malloc_region_init(new_zone->regions, actual_size -
			sizeof(t_malloc_zone) - sizeof(t_region));
	new_zone->size = actual_size;
	new_zone->num_regions = actual_size / region_size;
	new_zone->region_size = region_size;
	new_zone->next_zone = NULL;
	return (new_zone);
}

t_malloc_zone	*malloc_alloc_zone_of_type(t_zone_type type, size_t size)
{
	t_malloc_zone	*zone;

	if (type == Z_TINY)
		zone = malloc_alloc_zone(MIN_REGS_IN_TS_ZONES, TINY_REGION_MAX_SIZE);
	else if (type == Z_SMALL)
		zone = malloc_alloc_zone(MIN_REGS_IN_TS_ZONES, SMALL_REGION_MAX_SIZE);
	else
		zone = malloc_alloc_zone(1, size);
	if (zone)
		zone->type = type;
	return (zone);
}

int				malloc_zone_can_dealloc(t_malloc_zone *zone)
{
	return (zone->regions->free &&
			zone->regions->age == 0 &&
			zone->regions->next == NULL);
}

void			malloc_dealloc_zone(t_malloc_zone *zone)
{
#if 0
	t_region		*region;

	region = zone->regions;
	while (region)
	{
		assert(region->free);
		assert(region->age == 0);
		region = region->next;
	}
#endif
	munmap(zone, zone->size);
}
