#ifndef __FT_MALLOC_INTERNAL_H
# define __FT_MALLOC_INTERNAL_H

# include <pthread.h>
# include "ft_malloc.h"
# include "malloc_zone.h"

typedef struct	s_malloc_mdata
{
	pthread_mutex_t	lock;
	t_malloc_zone	*tiny_zones;
	t_malloc_zone	*small_zones;
	t_malloc_zone	*large_zones;
	unsigned char	initialized;
}				t_malloc_mdata;

t_malloc_mdata	*get_malloc_mdata(void);
void			*malloc_alloc_region(size_t size);

t_region_err	malloc_region_gc_run(void);

void			malloc_validity_report(void);

#endif
