/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 13:37:27 by nayara            #+#    #+#             */
/*   Updated: 2026/01/21 12:52:58 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	clear_gnl(int fd)
{
	char	*tmp;

	tmp = get_next_line(fd);
	while (tmp)
	{
		free(tmp);
		tmp = get_next_line(fd);
	}
}

int	is_empty_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] != ' ' && line[i] != '\t'
			&& line[i] != '\n' && line[i] != '\r')
			return (0);
		i++;
	}
	return (1);
}

int	process_line(t_game *g, char *line)
{
	char	*trimmed;
	int		res;

	res = 0;
	trimmed = ft_strtrim(line, " \t\n\r");
	if (!trimmed || !*trimmed)
		return (free(trimmed), 1);
	if (!ft_strncmp(trimmed, "NO ", 3))
		res = save_texture(g, NO, trimmed);
	else if (!ft_strncmp(trimmed, "SO ", 3))
		res = save_texture(g, SO, trimmed);
	else if (!ft_strncmp(trimmed, "WE ", 3))
		res = save_texture(g, WE, trimmed);
	else if (!ft_strncmp(trimmed, "EA ", 3))
		res = save_texture(g, EA, trimmed);
	else if (!ft_strncmp(trimmed, "F ", 2))
		res = save_color(g, FLOOR, trimmed);
	else if (!ft_strncmp(trimmed, "C ", 2))
		res = save_color(g, CEILING, trimmed);
	else
		res = -1;
	free(trimmed);
	return (res);
}
