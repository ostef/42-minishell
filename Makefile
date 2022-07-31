NAME = minishell
SRC_DIR = source
SRC_FILES = main.c shell.c error.c signal.c\
	env/env.c env/parse.c\
	parse/parse.c parse/args.c parse/redirs_parse.c parse/post_process.c\
	exec/exec_line.c exec/exec_cmd.c exec/find.c exec/builtin.c exec/redirs.c\
	builtins/dir.c builtins/shell.c builtins/export_builtin.c

ifeq ($(shell uname -s),Darwin)

RL_LIB = -L$(shell brew --prefix readline)/lib
RL_INC = -I$(shell brew --prefix readline)/include

else

RL_LIB = -L/usr/lib/x86_64-linux-gnu/
RL_INC = -I/usr/include/readline

endif

TESTS = abort bus_error fp_except segfault
OBJ_DIR = obj
OBJ_FILES = $(SRC_FILES:.c=.o)
INCLUDE_DIRS = libft .
DEPENDENCIES = minishell.h source/exec/exec.h source/parse/parse.h libft/libft.a Makefile
LIB_DIRS = libft
LIBS = ft readline
CC = gcc
C_FLAGS = $(addprefix -I, $(INCLUDE_DIRS)) -Wall -Wextra -Werror #-fsanitize=address -g
TESTS_C_FLAGS = -Wall -Wextra -Werror

all: libft $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPENDENCIES)
	@mkdir -p $(@D)
	$(CC) $(C_FLAGS) $(RL_INC) -c $< -o $@

$(NAME): $(addprefix $(OBJ_DIR)/, $(OBJ_FILES))
	$(CC) $(C_FLAGS) $(RL_INC) $(RL_LIB) -lreadline $(addprefix $(OBJ_DIR)/, $(OBJ_FILES)) $(addprefix -L, $(LIB_DIRS)) $(addprefix -l, $(LIBS)) $(RL_INC) -o $(NAME)

tests/%: tests/%.c Makefile
	$(CC) $(TESTS_C_FLAGS) $< -o $@

tests: $(addprefix tests/,$(TESTS))

libft:
	@ $(MAKE) -C libft

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)
	rm -f $(addprefix tests/,$(TESTS))

fclean_libs:
	@ $(MAKE) -C libft fclean

re: fclean all

.PHONY: all libft clean fclean tests
