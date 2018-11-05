#include "malloc_internal.h"

static inline t_malloc_zone	**get_zone_by_size(t_malloc_mdata *md, size_t size)
{
	if (size <= TINY_REGION_MAX_SIZE)
		return (&md->tiny_zones);
	else if (size <= SMALL_REGION_MAX_SIZE)
		return (&md->small_zones);
	else
		return (&md->large_zones);
}

void	*malloc_region_alloc(size_t size)
{
	t_malloc_mdata	*mdata;
	t_malloc_zone	**zone;

	mdata = get_malloc_mdata();
	zone = get_zone_by_size(mdata, size);

	return (NULL);
}
