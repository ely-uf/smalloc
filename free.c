#include "ft_malloc.h"
#include "malloc_internal.h"
# include <stdlib.h>

void	free(void *ptr)
{
	t_region	*region;

	if (!ptr)
	{
		malloc_region_gc_run();
		return ;
	}

	region = (t_region*)((char*)ptr - sizeof(t_region));
	if (malloc_region_free(region) != R_SUCCESS)
		abort();

	malloc_region_gc_run();
}
