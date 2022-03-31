NAME = minishell
SRC_DIR = source
SRC_FILES = main.c error.c env.c\
	parse/parse.c parse/args.c parse/redirs_parse.c parse/post_process.c\
	exec/exec.c exec/find.c exec/builtin.c exec/redirs.c\
	builtins/dir.c builtins/shell.c
OBJ_DIR = obj
OBJ_FILES = $(SRC_FILES:.c=.o)
INCLUDE_DIRS = libft .
DEPENDENCIES = minishell.h libft/libft.a Makefile
LIB_DIRS = libft
LIBS = ft readline
CC = gcc
C_FLAGS = $(addprefix -I, $(INCLUDE_DIRS)) #-fsanitize=address -g #-Wall -Wextra -Werror

all: | libft $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPENDENCIES)
	@mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< -o $@

$(NAME): $(addprefix $(OBJ_DIR)/, $(OBJ_FILES))
	$(CC) $(C_FLAGS) $(addprefix $(OBJ_DIR)/, $(OBJ_FILES)) $(addprefix -L, $(LIB_DIRS)) $(addprefix -l, $(LIBS)) -o $(NAME)

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
