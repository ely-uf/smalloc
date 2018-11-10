#ifndef __MALLOC_REGION_H
# define __MALLOC_REGION_H

# include <unistd.h>

# define MAGIC_START_XOR_MASK 0x1743
# define MAGIC_END_XOR_MASK 0xF795
# define MAGIC_START(addr_) (((addr_) & 0xFFFF) ^ MAGIC_START_XOR_MASK)
# define MAGIC_END(addr_) (((addr_) >> 48) ^ MAGIC_END_XOR_MASK)
# define FREE_REGION_AGE	5
# define ROUND_TO(n_, r_) ((n_) % (r_) ? (n_) + ((r_) - ((n_) % (r_))) : (n_))
# define ALIGN_SIZE(sz_) ROUND_TO((sz_), 4)

typedef enum	e_region_err
{
	R_SUCCESS,
	R_REGION_INVALID,
	R_REGION_CLAIMED,
	R_NOT_ENOUGH_SIZE,
}				t_region_err;

typedef struct	s_region
{
	unsigned short	magic_start;
	size_t			used_size;
	size_t			size;
	struct s_region	*prev;
	struct s_region	*next;
	unsigned short	free : 1;
	unsigned short	age : 3;
	unsigned short	magic_end;
}				t_region;

void			malloc_region_magic_set(t_region *region);
int				malloc_region_magic_valid(t_region *region);

t_region_err	malloc_region_init(t_region	*region, size_t size);
t_region_err	malloc_region_destroy(t_region *region);
t_region_err	malloc_region_claim(t_region *region, size_t size);
t_region_err	malloc_region_extend(t_region *region, size_t size);
t_region_err	malloc_region_free(t_region *region);

t_region_err	malloc_region_can_extend(t_region *region, size_t pref_sz);
t_region_err	malloc_region_can_claim(t_region *region, size_t size);

#endif
