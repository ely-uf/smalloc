#include "ft_malloc.h"
#include "malloc_internal.h"
# include <string.h>

void	*realloc(void *ptr, size_t size)
{
	t_region	*region;
	void		*new_ptr;

	if (!ptr)
		return (malloc(size));
	if (!size)
		return (NULL);
	region = (t_region*)((char*)ptr - sizeof(t_region));

	if (malloc_region_can_extend(region, size) == R_SUCCESS)
	{
		malloc_region_extend(region, size);
		return ((void*)(region + 1));
	}

	new_ptr = malloc(size);
	if (!new_ptr)
		return (NULL);

	memcpy(new_ptr, ptr, region->used_size > size ?
			size : region->used_size);
	malloc_region_free(region);
	return (new_ptr);
}
