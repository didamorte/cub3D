/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 12:31:00 by nayara            #+#    #+#             */
/*   Updated: 2026/01/21 12:53:40 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

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

static int	parse_map_file(t_game *g, int fd)
{
	char	*map_buffer;
	int		map_error;

	map_buffer = read_map_to_string(g, fd);
	if (!map_buffer)
		return (printf("Error\nMap is missing\n"), 0);
	g->map = ft_split(map_buffer, '\n');
	free(map_buffer);
	map_error = check_map_chars(g);
	if (map_error == 1)
		return (printf("Error\nInvalid character in map\n"), 0);
	if (map_error == 2)
		return (printf("Error\nNo player found\n"), 0);
	if (map_error == 3)
		return (printf("Error\nMultiple players found\n"), 0);
	if (!is_map_closed(g))
		return (printf("Error\nMap is not closed by walls\n"), 0);
	return (1);
}

int	parse_cub_file(t_game *g, char *file_path)
{
	int	fd;

	fd = open(file_path, O_RDONLY);
	if (fd < 0)
		return (printf("Error\nCould not open file\n"), 0);
	if (!parse_identifiers(g, fd))
		return (drain_gnl(fd), free_textures_paths(g), close(fd), 0);
	if (!parse_map_file(g, fd))
		return (free_matrix(g->map), free_textures_paths(g), close(fd), 0);
	close(fd);
	return (1);
}
