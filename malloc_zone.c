#include "malloc_zone.h"

#include <assert.h>

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
	while (actual_size < min_size)
		actual_size += page_size;
	new_zone = mmap(NULL, actual_size, PROT_ALL, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (new_zone == MAP_FAILED)
		return (NULL);
	new_zone->regions = (t_region *)(new_zone + 1);
	malloc_region_init(new_zone->regions, actual_size - sizeof(t_malloc_zone));
	new_zone->size = actual_size;
	new_zone->num_regions = actual_size / region_size;
	new_zone->region_size = region_size;
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

void			malloc_dealloc_zone(t_malloc_zone **zone_)
{
	t_region		*region;
	t_malloc_zone	*zone;

	zone = *zone_;
	region = zone->regions;
	while (region)
	{
		assert(region->free);
		region = region->next;
	}
	munmap(zone, zone->size);
	*zone_ = NULL;
}