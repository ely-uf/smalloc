#include "malloc_region.h"
#include "malloc_internal.h"
# include <stdio.h>
# include <assert.h>

int		malloc_region_check(t_region *region)
{
	if (!malloc_region_magic_valid(region))
	{
		printf("Invalid region: %p\n", region);
		printf("Magic should be: %hu : %hu\n",
			(unsigned short)MAGIC_START(((unsigned long long)(region))),
			(unsigned short)MAGIC_END(((unsigned long long)(region))));
		printf("Magic really is: %hu : %hu\n", region->magic_start,
				region->magic_end);
		return (1);
	}
	return (0);
}

int		malloc_zone_check(t_malloc_zone *zone)
{
	t_region	*region_iter;
	int			res;

	res = 0;
	region_iter = zone->regions;
	while (region_iter)
	{
		if (malloc_region_check(region_iter))
			res = 1;
		region_iter = region_iter->next;
	}
	return (res);
}

void	malloc_validity_report(void)
{
	t_malloc_mdata	*mdata;
	t_malloc_zone	*zone_iter;

	mdata = get_malloc_mdata();

	printf("Checking tiny zones:\n");
	zone_iter = mdata->tiny_zones;
	while (zone_iter)
	{
		printf("%s\n", malloc_zone_check(zone_iter) ? "FAIL" : "OK");
		zone_iter = zone_iter->next_zone;
	}

	printf("Checking small zones:\n");
	zone_iter = mdata->small_zones;
	while (zone_iter)
	{
		printf("%s\n", malloc_zone_check(zone_iter) ? "FAIL" : "OK");
		zone_iter = zone_iter->next_zone;
	}

	printf("Checking large zones:\n");
	zone_iter = mdata->large_zones;
	while (zone_iter)
	{
		printf("%s\n", malloc_zone_check(zone_iter) ? "FAIL" : "OK");
		zone_iter = zone_iter->next_zone;
	}
}
