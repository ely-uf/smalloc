#ifndef __MALLOC_ZONE_H
# define __MALLOC_ZONE_H

# include <unistd.h>
# include <sys/mman.h>
# include "malloc_region.h"

# define TINY_REGION_MAX_SIZE	128
# define SMALL_REGION_MAX_SIZE	2048
# define MIN_REGS_IN_TS_ZONES	32
# define DEFAULT_ZONE_SIZE	(-1)

# define PROT_ALL (PROT_READ | PROT_WRITE | PROT_EXEC)

typedef enum	e_zone_type
{
	Z_TINY,
	Z_SMALL,
	Z_LARGE
}				t_zone_type;

typedef struct	s_malloc_zone
{
	t_zone_type				type;
	t_region				*regions;
	size_t					num_regions;
	size_t					region_size;
	size_t					size;
	struct s_malloc_zone	*next_zone;
}				t_malloc_zone;

t_malloc_zone	*malloc_alloc_zone_of_type(t_zone_type type, size_t size);
int				malloc_zone_can_dealloc(t_malloc_zone *zone);
void			malloc_dealloc_zone(t_malloc_zone *zone);

#endif
