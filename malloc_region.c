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

t_region_err		malloc_region_can_claim(t_region *region, size_t size)
{
	if (!malloc_region_valid(region))
		return (R_REGION_INVALID);
	if (!region->free)
		return (R_REGION_CLAIMED);
	if (region->size < ALIGN_SIZE(size))
		return (R_NOT_ENOUGH_SIZE);
	return (R_SUCCESS);
}

t_region_err		malloc_region_claim(t_region *region, size_t size)
{
	t_region		*region_after;
	size_t			size_left;
	t_region_err	claim_res;
	size_t			aligned_size;

	claim_res = malloc_region_can_claim(region, size);
	if (claim_res != R_SUCCESS)
		return (claim_res);
	aligned_size = ALIGN_SIZE(size);
	region->used_size = size;
	region->free = 0;
	size_left = region->size - aligned_size;
	if (size_left >= sizeof(t_region) + 4)
	{
		region->size = aligned_size;
		region_after = (t_region*)((char*)(region + 1) + region->size);
		malloc_region_init(region_after, size_left - sizeof(t_region));
		region_after->next = region->next;
		if (region_after->next)
			region_after->next->prev = region_after;
		region_after->prev = region;
		region->next = region_after;
	}
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
		return (malloc_region_can_extend(region->next, pref_sz - region->size));
}

t_region_err		malloc_region_extend(t_region *region, size_t pref_sz)
{
	t_region_err	ret;

	ret = malloc_region_can_extend(region, pref_sz);
	if (ret != R_SUCCESS)
		return (ret);
	while (region->size < pref_sz)
		malloc_region_destroy(region->next);
	region->used_size = pref_sz;
	return (R_SUCCESS);
}

t_region_err		malloc_region_destroy(t_region *region)
{
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
	prev = region->prev;
	prev->next = region->next;
	prev->size += region->size + sizeof(t_region);
	if (prev->next)
		prev->next->prev = prev;
	region->magic_start = 0;
	region->magic_end = 0;
	return (R_SUCCESS);
}
