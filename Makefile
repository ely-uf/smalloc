ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
CONST_NAME = libft_malloc.so
SRC = malloc.c realloc.c free.c malloc_zone.c malloc_region.c \
	malloc_region_gc.c malloc_region_magic.c malloc_metadata.c\
	malloc_allocation.c
OBJ = $(SRC:.c=.o)
CFLAGS = -fPIC -Wall -Werror -Wextra
CC = gcc
LN = ln

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -shared $(OBJ) -o $(NAME)
	$(LN) -sf $(NAME) $(CONST_NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) $(CONST_NAME)

re: fclean all
