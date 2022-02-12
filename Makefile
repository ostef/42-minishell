NAME = minishell
SRC_FILES = source/main.c source/parse.c source/cmd.c
OBJ_FILES = $(SRC_FILES:.c=.o)
INCLUDE_FILES = minishell.h
INCLUDE_DIRS = libft .
DEPENDENCIES = libft/libft.a
LIB_DIRS = libft
LIBS = ft readline
CC = gcc
C_FLAGS = $(addprefix -I, $(INCLUDE_DIRS)) #-Wall -Wextra -Werror

all: | libft $(NAME)

$(OBJ_FILES): %.o: %.c $(INCLUDE_FILES) $(DEPENDENCIES) Makefile
	$(CC) $(C_FLAGS) -c $< -o $@

$(NAME): $(OBJ_FILES)
	$(CC) $(addprefix -L, $(LIB_DIRS)) $(addprefix -l, $(LIBS)) -framework OpenGL -framework AppKit $(OBJ_FILES) -o $(NAME)

libft:
	@make -C libft

clean:
	rm -f $(OBJ_FILES)

fclean: clean
	rm -f $(NAME)

fclean_libs:
	make -C libft fclean

re: fclean all

.PHONY: all libft clean fclean
