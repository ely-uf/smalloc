#ifndef __FT_MALLOC_H
# define __FT_MALLOC_H

# include <unistd.h>

void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);

void	show_alloc_mem(void);

#endif
