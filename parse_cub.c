/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 18:58:03 by nayara            #+#    #+#             */
/*   Updated: 2026/01/20 20:26:15 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	parse_identifiers(t_game *g, int fd)
{
	char	*line;
	int		res;
	int		found_identifiers;

	found_identifiers = 0;
	line = get_next_line(fd);
	while (line)
	{
		if (is_empty_line(line))
		{
			free(line);
			line = get_next_line(fd);
			continue ;
		}
		res = process_line(g, line);
		if (res == 1)
			found_identifiers++;
		else if (res == 0)
		{
			free(line);
			return (printf("Error\nInvalid configuration element\n"), 0);
		}
		else if (res == -1)
		{
			if (found_identifiers < 6)
			{
				free(line);
				return (printf("Error\nMissing identifiers before map\n"), 0);
			}
			g->map_line = line;
			return (1);
		}
		free(line);
		line = get_next_line(fd);
	}
	return (found_identifiers == 6);
}

int	check_map_chars(t_game *g)
{
	int	y;
	int	x;
	int	p_count;

	y = 0;
	p_count = 0;
	while (g->map[y])
	{
		x = 0;
		while (g->map[y][x])
		{
			if (g->map[y][x] == '\t' || g->map[y][x] == '\r')
				g->map[y][x] = ' ';
			if (!ft_strchr("01NSEW ", g->map[y][x]))
				return (0);
			if (ft_strchr("NSEW", g->map[y][x]))
				p_count++;
			x++;
		}
		y++;
	}
	return (p_count == 1);
}

int	is_map_closed(t_game *g)
{
	int	x;
	int	y;

	y = 0;
	while (g->map[y])
	{
		x = 0;
		while (g->map[y][x])
		{
			if (ft_strchr("0NSEW", g->map[y][x]))
			{
				if (y == 0 || !g->map[y + 1] || x == 0 || g->map[y][x + 1] == '\0')
					return (0);
				if (g->map[y - 1][x] == ' ' || g->map[y][x - 1] == ' '
					|| g->map[y + 1][x] == ' ' || g->map[y][x + 1] == ' ')
					return (0);
				if (x >= (int)ft_strlen(g->map[y - 1])
					|| x >= (int)ft_strlen(g->map[y + 1]))
					return (0);
			}
			x++;
		}
		y++;
	}
	return (1);
}

char	*read_map_to_string(t_game *g, int fd)
{
	char	*line;
	char	*tmp;
	char	*full_str;

	if (!g->map_line)
		return (NULL);
	full_str = ft_strdup(g->map_line);
	free(g->map_line);
	g->map_line = NULL;
	line = get_next_line(fd);
	while (line)
	{
		tmp = full_str;
		full_str = ft_strjoin(tmp, line);
		free(tmp);
		free(line);
		line = get_next_line(fd);
	}
	return (full_str);
}

int	parse_cub_file(t_game *g, char *file_path)
{
	int		fd;
	char	*map_buffer;

	fd = open(file_path, O_RDONLY);
	if (fd < 0)
		return (printf("Error\nCould not open file\n"), 0);
	if (!parse_identifiers(g, fd))
	{
		close(fd);
		return (0);
	}
	map_buffer = read_map_to_string(g, fd);
	if (!map_buffer)
	{
		close(fd);
		return (printf("Error\nMap is missing\n"), 0);
	}
	g->map = ft_split(map_buffer, '\n');
	free(map_buffer);
	close(fd);
	if (!check_map_chars(g))
		return (printf("Error\nInvalid characters, \
multiple players or NO player\n"), 0);
	if (!is_map_closed(g))
		return (printf("Error\nMap is not closed by walls\n"), 0);
	return (1);
}
