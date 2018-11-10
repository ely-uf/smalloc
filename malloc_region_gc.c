#include "malloc_region.h"
#include "malloc_internal.h"
# include <assert.h>

static inline void	malloc_region_gc_collect(t_region *region)
{
	if (region->age)
	{
		region->age--;
		return ;
	}
	malloc_region_destroy(region);
}

t_region_err		malloc_region_gc_regions(t_region *region)
{
	t_region	*next;

	while (region)
	{
		next = region->next;
		if (region->free)
			malloc_region_gc_collect(region);
		region = next;
	}
	return (R_SUCCESS);
}

t_region_err		malloc_region_gc_zone_cleanup(t_malloc_zone **zone_)
{
	t_malloc_zone	*prev;
	t_malloc_zone	*next;
	t_malloc_zone	*zone;

	prev = *zone_;
	zone = *zone_;
	while (zone)
	{
		next = zone->next_zone;
		if (malloc_zone_can_dealloc(zone))
		{
			if (zone == *zone_)
				*zone_ = (*zone_)->next_zone;
			else
				prev->next_zone = next;
			malloc_dealloc_zone(zone);
		}
		else
		{
			prev = zone;
		}
		zone = next;
	}
	return (R_SUCCESS);
}

t_region_err		malloc_region_gc_zone_list(t_malloc_zone **zone_,
		int destroy_first)
{
	t_malloc_zone	*zone;

	if (!*zone_)
		return (R_SUCCESS);
	zone = *zone_;
	while (zone)
	{
		malloc_region_gc_regions(zone->regions);
		zone = zone->next_zone;
	}
	malloc_region_gc_zone_cleanup(&(*zone_)->next_zone);
	if (destroy_first && malloc_zone_can_dealloc(*zone_))
	{
		malloc_dealloc_zone(*zone_);
		*zone_ = NULL;
	}
	return (R_SUCCESS);
}

t_region_err		malloc_region_gc_run(void)
{
	t_malloc_mdata	*mdata;

	mdata = get_malloc_mdata();
	malloc_region_gc_zone_list(&mdata->tiny_zones, 0);
	malloc_region_gc_zone_list(&mdata->small_zones, 0);
	malloc_region_gc_zone_list(&mdata->large_zones, 1);
	return (R_SUCCESS);
}
