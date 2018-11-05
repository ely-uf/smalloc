#include "malloc_region.h"

static inline void	malloc_region_gc_collect(t_region **region, t_region *prev)
{
	if ((*region)->age)
	{
		(*region)->age--;
		return ;
	}
	malloc_region_destroy(*region);
	*region = prev;
}

t_region_err		malloc_region_gc_run(t_region *region)
{
	t_region	*prev;

	prev = region;
	while (region)
	{
		if (region->free)
			malloc_region_gc_collect(&region, prev);
		else
			prev = region;
		region = region->next;
	}
	return (R_SUCCESS);
}
