#include "malloc_region.h"

static inline int	malloc_region_valid(t_region *region)
{
	return (malloc_region_magic_valid(region));
}

t_region_err		malloc_region_init(t_region *region, size_t size)
{
	malloc_region_magic_set(region);
	region->used_size = 0;
	region->size = size;
	region->free = 1;
	region->age = FREE_REGION_AGE;
	region->prev = NULL;
	region->next = NULL;
	return (R_SUCCESS);
}

t_region_err		malloc_region_free(t_region *region)
{
	if (!malloc_region_valid(region))
		return (R_REGION_INVALID);
	region->free = 1;
	region->age = FREE_REGION_AGE;
	return (R_SUCCESS);
}

t_region_err		malloc_region_can_extend(t_region *region, size_t pref_sz)
{
	if (!malloc_region_valid(region))
		return (R_REGION_INVALID);
	if (!region->next || !region->next->free)
		return (R_NOT_ENOUGH_SIZE);
	if (region->size + region->next->size > pref_sz)
		return (R_SUCCESS);
	else
		return (R_NOT_ENOUGH_SIZE);
}

t_region_err		malloc_region_destroy(t_region *region)
{
	size_t		size;
	t_region	*prev;

	if (!malloc_region_valid(region))
		return (R_REGION_INVALID);
	/*
	 *	First region in zone.
	 *	Leave it alone, just free.
	 */
	if (!region->prev)
	{
		region->free = 1;
		return (R_SUCCESS);
	}
	size = region->size;
	prev = region->prev;
	prev->next = region->next;
	if (prev->next)
		prev->next->prev = prev;
	region->magic_start = 0;
	region->magic_end = 0;
	return (R_SUCCESS);
}
