#include "malloc_region.h"

void	malloc_region_magic_set(t_region *region)
{
	unsigned long long addr;

	addr = (unsigned long long)region;
	region->magic_start = MAGIC_START(addr);
	region->magic_end = MAGIC_END(addr);
}

int		malloc_region_magic_valid(t_region *region)
{
	unsigned long long addr;

	addr = (unsigned long long)region;
	return (region->magic_start == MAGIC_START(addr) &&
			region->magic_end == MAGIC_END(addr));
}
