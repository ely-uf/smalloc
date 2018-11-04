#include "malloc_internal.h"

static t_malloc_mdata metadata;

static int _malloc_mdata_initialize(t_malloc_mdata *md)
{
	
	return (0);
}

static inline t_malloc_mdata *_get_malloc_mdata(void)
{
	if (!metadata.initialized && _malloc_mdata_initialize(&metadata) != 0)
		return (NULL);
	return (&metadata);
}
