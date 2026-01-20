/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 19:09:59 by nayara            #+#    #+#             */
/*   Updated: 2026/01/20 16:20:50 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	parse_identifiers(t_game *g, int fd)
{
	char	*line;
	int		found_elements;

	found_elements = 0;
	while (found_elements < 6)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (is_empty_line(line))
		{
			free(line);
			continue ;
		}
		if (!process_line(g, line))
		{
			free(line);
			clear_gnl(fd);
			free_textures_paths(g);
			return (printf("Error\nInvalid configuration element\n"), 0);
		}
		found_elements++;
		free(line);
	}
	return (found_elements == 6);
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

char	*read_map_to_string(int fd)
{
	char	*line;
	char	*tmp;
	char	*full_str;

	full_str = ft_strdup("");
	line = get_next_line(fd);
	while (line && is_empty_line(line))
	{
		free(line);
		line = get_next_line(fd);
	}
	while (line)
	{
		tmp = full_str;
		full_str = ft_strjoin(tmp, line);
		free(tmp);
		free(line);
		line = get_next_line(fd);
	}
	if (ft_strlen(full_str) == 0)
		return (free(full_str), NULL);
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
	map_buffer = read_map_to_string(fd);
	if (!map_buffer)
		return (printf("Error\nMap is missing\n"));
	g->map = ft_split(map_buffer, '\n');
	free(map_buffer);
	close(fd);
	if (!check_map_chars(g))
		return (printf("Error\nInvalid characters or multiple players\n"), 0);
	if (!is_map_closed(g))
		return (printf("Error\nMap is not closed by walls\n"), 0);
	return (1);
}
