/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 13:37:27 by nayara            #+#    #+#             */
/*   Updated: 2026/01/20 16:16:55 by nayara           ###   ########.fr       */
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
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
			return (0);
		i++;
	}
	return (1);
}

int	process_line(t_game *g, char *line)
{
	if (!ft_strncmp(line, "NO ", 3) && !g->textures[NO].img)
		return (save_texture(g, NO, line + 3));
	if (!ft_strncmp(line, "SO ", 3) && !g->textures[SO].img)
		return (save_texture(g, SO, line + 3));
	if (!ft_strncmp(line, "WE ", 3) && !g->textures[WE].img)
		return (save_texture(g, WE, line + 3));
	if (!ft_strncmp(line, "EA ", 3) && !g->textures[EA].img)
		return (save_texture(g, EA, line + 3));
	if (!ft_strncmp(line, "F ", 2) && g->floor_color == -1)
		return (save_color(g, FLOOR, line + 2));
	if (!ft_strncmp(line, "C ", 2) && g->ceiling_color == -1)
		return (save_color(g, CEILING, line + 2));
	return (0);
}
