#include "malloc_internal.h"

void	*malloc(size_t size)
{
	void	*res_ptr;

	if (size == 0)
		return (NULL);
	res_ptr = malloc_alloc_region(size);
	return (res_ptr);
}
