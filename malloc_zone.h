#ifndef __MALLOC_ZONE_H
# define __MALLOC_ZONE_H

# include <unistd.h>

typedef void * t_region;

typedef enum	e_zone_type
{
	Z_TINY,
	Z_SMALL,
	Z_LARGE
}				t_zone_type;

typedef struct	s_malloc_zone
{
	t_region				*regions;
	size_t					num_regions;
	size_t					region_size;
	struct s_malloc_zone	*next_zone;
}				t_malloc_zone;

#endif
