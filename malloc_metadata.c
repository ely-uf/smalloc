#include "malloc_internal.h"

static t_malloc_mdata	g_metadata;

static int						malloc_mdata_initialize(t_malloc_mdata *md)
{
	if (!(md->tiny_zones = malloc_alloc_zone_of_type(Z_TINY,
					DEFAULT_ZONE_SIZE)))
		return (1);
	if (!(md->small_zones = malloc_alloc_zone_of_type(Z_SMALL,
					DEFAULT_ZONE_SIZE)))
	{
		malloc_dealloc_zone(md->tiny_zones);
		md->tiny_zones = NULL;
		return (1);
	}
	md->initialized = 1;
	return (0);
}

static inline t_malloc_mdata	*get_malloc_mdata_(void)
{
	if (!g_metadata.initialized && malloc_mdata_initialize(&g_metadata) != 0)
		return (NULL);
	return (&g_metadata);
}

t_malloc_mdata					*get_malloc_mdata(void)
{
	return (get_malloc_mdata_());
}
