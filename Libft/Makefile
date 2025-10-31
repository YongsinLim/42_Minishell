# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yolim <yolim@student.42kl.edu.my>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/31 21:25:42 by yolim             #+#    #+#              #
#    Updated: 2025/07/30 17:21:02 by yolim            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = cc
CCFLAGS = -Wall -Wextra -Werror
RM = rm -f
AR = ar rcs

SRC = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
		ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
		ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
		ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
		ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c \
		ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
		ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c
OBJS := $(SRC:%.c=%.o)

BONUS_SRC = ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
			ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c \
			ft_lstmap.c
BONUS_OBJS := $(BONUS_SRC:%.c=%.o)

PRINTF = ft_itoa_base.c ft_utoa_base.c ft_print_helper.c ft_printf.c
PRINTF_OBJS := $(PRINTF:%.c=%.o)

GNL_BONUS_SRC = get_next_line_bonus.c get_next_line_utils_bonus.c
GNL_BONUS_OBJS := $(GNL_BONUS_SRC:%.c=%.o)

all: $(NAME)

$(NAME): $(OBJS) $(BONUS_OBJS) $(PRINTF_OBJS) $(GNL_BONUS_OBJS)
	$(AR) $(NAME) $(OBJS) $(BONUS_OBJS) $(PRINTF_OBJS) $(GNL_BONUS_OBJS)

libft: $(OBJS)
	$(AR) $(NAME) $(OBJS)

bonus: $(OBJS) $(BONUS_OBJS)
	$(AR) $(NAME) $(OBJS) $(BONUS_OBJS)

printf: $(PRINTF_OBJS)
	$(AR) $(NAME) $(PRINTF_OBJS)

gnl: $(GNL_BONUS_OBJS)
	$(AR) $(NAME) $(GNL_BONUS_OBJS)

clean:
	$(RM) $(OBJS) $(BONUS_OBJS) $(PRINTF_OBJS) $(GNL_BONUS_OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

%.o: %.c
	$(CC) $(CPPFLAGS) $(CCFLAGS) -o $@ -c $<

# -I = tells compiler "look in this directory for header files"
# . means current directory
CPPFLAGS = -I.

.PHONY: all libft bonus printf gnl clean fclean re