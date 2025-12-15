# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/14 16:21:19 by yolim             #+#    #+#              #
#    Updated: 2025/12/15 12:43:00 by yolim            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ----- Executable name ----- 
NAME = minishell
BONUS_NAME = minishell_bonus

# ----- Compiler and flags ----- 
CC = cc
CCFLAGS = -Wall -Wextra -Werror
RM = rm -f

# ----- Header file ----- 
HEADER_DIR = ./Includes
HEADER_LIB = $(HEADER_DIR)/minishell.h

# ----- Source Files ----- 
SRC = 
OBJS := $(SRC:%.c=%.o)

BONUS_SRC = 
BONUS_OBJS := $(BONUS_SRC:%.c=%.o)
## if got do bonus

# ----- Libft ----- 
LIBFT_DIR = ./Libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a

# ----- Linker Flags ----- 
LIBFT_FLAGS = -L$(LIBFT_DIR) -lft

# ----- Rules ----- 
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_LIB)
	$(CC) $(CCFLAGS) $(OBJS) $(LIBFT_FLAGS) -o $(NAME)
	@echo "Done. Executable is '$(NAME)'"

bonus: $(BONUS_OBJS) $(LIBFT_LIB)
	$(CC) $(CCFLAGS) $(BONUS_OBJS) $(LIBFT_FLAGS) -o $(BONUS_NAME)
	@echo "Done. Bonus executable is '$(BONUS_NAME)'"

# Rule to build libft library
$(LIBFT_LIB):
	@make -s -C $(LIBFT_DIR)

# Rule to compile source files into object files
# -I stands for "Include" & tells the compiler where to look for header files
# $< is the first prerequisite of the rule (main.c)
# $@ is the target of the rule (main.o)
%.o: %.c $(HEADER_LIB)
	@$(CC) $(CCFLAGS) -I$(HEADER_DIR) -I$(LIBFT_DIR) -c $< -o $@

# ----- Clean Rules ----- 
clean:
	@$(RM) $(OBJS) $(BONUS_OBJS)
	@echo "Object files cleaned."
	@make -sC $(LIBFT_DIR) clean
	@echo "Object files in Libft cleaned."

fclean: clean
	@$(RM) $(NAME)
	@$(RM) $(BONUS_NAME)
	@make -sC $(LIBFT_DIR) fclean
	@echo "Full clean complete."

# ----- Rebuild Rules ----- 
re: fclean all

.PHONY: all clean fclean re bonus