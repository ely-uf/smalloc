#include "malloc_internal.h"

void	*malloc(size_t size)
{
	if (size == 0)
		return (NULL);

	return malloc_alloc_region(size);
}
