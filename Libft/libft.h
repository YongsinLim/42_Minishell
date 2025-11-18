/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenlee <jenlee@student.42kl.edu.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 10:15:27 by yolim             #+#    #+#             */
/*   Updated: 2025/11/04 22:16:09 by jenlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

// Include necessary standard headers
# include <stdio.h>
# include <stdlib.h> // for malloc & free function
# include <stdint.h> // For uintptr_t
# include <unistd.h> // for read() function
# include <string.h>
# include <stdarg.h> // Required for va_list, va_start, va_arg, va_end

// Libc Functions
int		ft_isalpha(int ch);
int		ft_isdigit(int nbr);
int		ft_isalnum(int ch);
int		ft_isascii(int c);
int		ft_isprint(int c);
size_t	ft_strlen(const char *str);
void	*ft_memset(void *ptr, int c, size_t n);
void	ft_bzero(void *s, size_t n);
void	*ft_memcpy(void *dest, const void *source, size_t n);
void	*ft_memmove(void *dest, const void *source, size_t n);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
int		ft_toupper(int ch);
int		ft_tolower(int ch);
char	*ft_strchr(const char *str, int ch);
char	*ft_strrchr(const char *str, int chr);
int		ft_strncmp(const char *str1, const char *str2, size_t count);
void	*ft_memchr(const void *ptr, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
char	*ft_strnstr(const char *big, const char *little, size_t len);
int		ft_atoi(const char *str);
void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strdup(const char *s);

// Libft Additional Functions
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
char	*ft_itoa(int n);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);

// Type definitions
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

// Libft Bonus Functions
t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

// Printf function
int		ft_printf(const char *str, ...);
char	*ft_itoa_base(int n, char *base_output);
char	*ft_utoa_base(unsigned long long num, char *base_output);
int		ft_putchar_getcount(char character);
int		ft_putstr_getcount(char *str);
int		ft_putint_getcount(int num);
int		ft_put_unbr_getcount(unsigned int num, char flag);
int		ft_putptr_getcount(uintptr_t p);

// Get Next Line Bonus Functions
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

typedef struct s_node
{
	int				fd;
	char			*read_content;
	struct s_node	*next;
}					t_node;

char	*get_next_line(int fd);
t_node	*get_or_create_node(int fd, t_node **head);
void	remove_fd(int fd, t_node **head);
void	revise_read_content(char *str, unsigned int start);
char	*get_result_line(t_node *current_node, ssize_t read_bytes,
			t_node **list_head);
char	*strmerge_free(char *str, char *buffer);
char	*substr(char const *s, size_t len);
void	free_gnl_list(t_node **head);

#endif