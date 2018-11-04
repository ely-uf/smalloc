#ifndef __FT_MALLOC_INTERNAL_H
# define __FT_MALLOC_INTERNAL_H

# include <pthread.h>
# include "ft_malloc.h"
# include "malloc_zone.h"

# define TINY_REGION_MAX_SIZE	128
# define SMALL_REGION_MAX_SIZE	4096
# define MIN_REGS_IN_TS_ZONES	4096
# define DEFAULT_ZONE_SIZE	(-1)

typedef struct	s_malloc_mdata
{
	pthread_mutex_t	lock;
	t_malloc_zone	*tiny_zones;
	t_malloc_zone	*small_zones;
	t_malloc_zone	*large_zones;	
	unsigned char	initialized;
}				t_malloc_mdata;

#endif
