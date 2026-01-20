/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 22:31:10 by rneto-fo          #+#    #+#             */
/*   Updated: 2026/01/19 17:13:38 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

char	*get_next_line(int fd);
char	*gnl_ft_strchr(const char *str, int c);
char	*gnl_ft_strjoin(char const *s1, char const *s2);
size_t	gnl_ft_strlen(const char *str);
char	*gnl_ft_substr(char const *s, unsigned int start, size_t len);
size_t	gnl_ft_strlcpy(char *dst, const char *src, size_t n);

#endif
