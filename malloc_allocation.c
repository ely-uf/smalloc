#include "malloc_internal.h"

# include <assert.h>

static inline t_malloc_zone	**get_zone_by_type(t_malloc_mdata *md, t_zone_type type)
{
	if (type == Z_TINY)
		return (&md->tiny_zones);
	else if (type == Z_SMALL)
		return (&md->small_zones);
	else
		return (&md->large_zones);
}

static inline t_malloc_zone **get_zone_by_type_(t_zone_type type)
{
	t_malloc_mdata	*mdata;

	mdata = get_malloc_mdata();
	if (!mdata)
		return (NULL);
	return (get_zone_by_type(mdata, type));
}

static inline t_zone_type	get_type_by_size(size_t size)
{
	if (size <= TINY_REGION_MAX_SIZE)
		return (Z_TINY);
	else if (size <= SMALL_REGION_MAX_SIZE)
		return (Z_SMALL);
	else
		return (Z_LARGE);
}

t_region	*malloc_alloc_region_in_zone(t_malloc_zone *zone, size_t size)
{
	t_region	*region_iter;

	region_iter = zone->regions;
	while (region_iter)
	{
		if (malloc_region_can_claim(region_iter, size) == R_SUCCESS)
		{
			assert(malloc_region_claim(region_iter, size) == R_SUCCESS);
			return (region_iter);
		}
		region_iter = region_iter->next;
	}
	return (NULL);
}

t_region	*malloc_alloc_region_in_zone_list(t_malloc_zone *zone_list,
		size_t size)
{
	t_region	*region;

	while (zone_list)
	{
		region = malloc_alloc_region_in_zone(zone_list, size);
		if (region)
			return (region);
		zone_list = zone_list->next_zone;
	}

	return (NULL);
}

void		*malloc_alloc_region(size_t size)
{
	t_malloc_zone	**zone;
	t_zone_type		type;
	t_malloc_zone	*zone_iter;
	t_region		*region;

	type = get_type_by_size(size);
	zone = get_zone_by_type_(type);
	if (!zone)
		return (NULL);
	if (!*zone && !(*zone = malloc_alloc_zone_of_type(type, size)))
		return (NULL);
	region = malloc_alloc_region_in_zone_list(*zone, size);
	if (region)
		return ((void*)(region + 1));
	zone_iter = *zone;
	while (zone_iter->next_zone)
		zone_iter = zone_iter->next_zone;
	if (!(zone_iter->next_zone = malloc_alloc_zone_of_type(type, size)))
		return (NULL);
	region = malloc_alloc_region_in_zone(zone_iter->next_zone, size);
	return (region ? ((void*)(region + 1)) : NULL);
}
