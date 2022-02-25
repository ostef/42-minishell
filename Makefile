NAME = minishell
SRC_DIR = source
SRC_FILES = main_cmd.c exec/exec.c
OBJ_DIR = obj
OBJ_FILES = $(SRC_FILES:.c=.o)
INCLUDE_DIRS = libft .
DEPENDENCIES = minishell.h libft/libft.a Makefile
LIB_DIRS = libft
LIBS = ft readline
CC = gcc
C_FLAGS = $(addprefix -I, $(INCLUDE_DIRS)) -Wall -Wextra -Werror

all: | libft $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPENDENCIES)
	@mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< -o $@

$(NAME): $(addprefix $(OBJ_DIR)/, $(OBJ_FILES))
	$(CC) $(addprefix -L, $(LIB_DIRS)) $(addprefix -l, $(LIBS)) $(addprefix $(OBJ_DIR)/, $(OBJ_FILES)) -o $(NAME)

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
