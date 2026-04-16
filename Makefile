# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/14 16:21:19 by yolim             #+#    #+#              #
#    Updated: 2026/04/17 02:25:49 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ----- Executable name ----- 
NAME = minishell

# ----- Compiler and flags ----- 
CC = cc
CCFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline -lhistory -L/opt/homebrew/opt/readline/lib
CPPFLAGS = -I/opt/homebrew/opt/readline/include
RM = rm -f

# ----- Header file ----- 
HEADER_DIR = ./includes
HEADER_LIB = $(HEADER_DIR)/minishell.h

# ----- Source Files -----
SRC = src/main.c \
	src/ast_build/command_or_subshell.c \
	src/ast_build/parse.c \
	src/ast_build/parse_helper.c \
	src/ast_build/parse_one_command.c \
	src/ast_build/parse_redirection.c \
	src/ast_build/tokens_to_cmd.c \
	src/ast_execute/ast_executor.c \
	src/ast_execute/build_path.c \
	src/ast_execute/directory.c \
	src/ast_execute/execute_simple_cmd.c \
	src/ast_execute/heredoc.c \
	src/ast_execute/heredoc_interactive.c \
	src/ast_execute/heredoc_noninteractive.c \
	src/ast_execute/redirection.c \
	src/ast_execute/wait_child.c \
	src/builtins/builtins.c \
	src/builtins/cd.c \
	src/builtins/echo.c \
	src/builtins/exit.c \
	src/builtins/export.c \
	src/builtins/pwd.c \
	src/builtins/unset.c \
	src/builtins/env/env.c \
	src/builtins/env/env_sort.c \
	src/builtins/env/env_utils.c \
	src/error_handling/error.c \
	src/free_memory/cleanup_exit.c \
	src/free_memory/free.c \
	src/general/general_helper.c \
	src/history/history.c \
	src/initialization/init.c \
	src/signals/signal_init.c \
	src/signals/signal_sets.c \
	src/tokens/expansion.c \
	src/tokens/main_tokenising.c \
	src/tokens/token_helper.c \
	src/tokens/token_words.c \
	src/tokens/token_words_main.c \
	src/unclosed_quotes/unclosed_quotes.c \
	src/wildcard/wildcard.c

OBJS := $(SRC:%.c=%.o)

# ----- Libft ----- 
LIBFT_DIR = ./Libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a

# ----- Linker Flags ----- 
LIBFT_FLAGS = -L$(LIBFT_DIR) -lft

# ----- Rules ----- 
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_LIB)
	$(CC) $(CCFLAGS) $(OBJS) $(LIBFT_FLAGS) $(LDFLAGS) -o $(NAME)
	@echo "Done. Executable is '$(NAME)'"

# Rule to build libft library
$(LIBFT_LIB):
	@make -s -C $(LIBFT_DIR)

# Rule to compile source files into object files
# -I stands for "Include" & tells the compiler where to look for header files
# $< is the first prerequisite of the rule (main.c)
# $@ is the target of the rule (main.o)
%.o: %.c $(HEADER_LIB)
	@$(CC) $(CCFLAGS) $(CPPFLAGS) -I$(HEADER_DIR) -I$(LIBFT_DIR) -c $< -o $@

# ----- Clean Rules ----- 
clean:
	@$(RM) $(OBJS)
	@echo "Object files cleaned."
	@make -sC $(LIBFT_DIR) clean
	@echo "Object files in Libft cleaned."

fclean: clean
	@$(RM) $(NAME)
	@make -sC $(LIBFT_DIR) fclean
	@echo "Full clean complete."

# ----- Rebuild Rules ----- 
re: fclean all

.PHONY: all clean fclean re