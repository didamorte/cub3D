/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_id.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 12:33:51 by nayara            #+#    #+#             */
/*   Updated: 2026/01/21 12:48:49 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	handle_identifier_line(t_game *g, char *line, int *found)
{
	int	res;

	res = process_line(g, line);
	if (res == 1)
		(*found)++;
	else if (res == 0)
		return (printf("Error\nInvalid configuration element\n"), 0);
	else if (res == -1)
	{
		if (*found < 6)
			return (printf("Error\nMissing identifiers before map\n"), 0);
		g->map_line = line;
		return (-1);
	}
	return (1);
}

static int	parse_identifiers_loop(t_game *g, int fd, int *found)
{
	char	*line;
	int		status;

	line = get_next_line(fd);
	while (line)
	{
		if (!is_empty_line(line))
		{
			status = handle_identifier_line(g, line, found);
			if (status == 0)
				return (free(line), 0);
			if (status == -1)
				return (1);
		}
		free(line);
		line = get_next_line(fd);
	}
	return (1);
}

int	parse_identifiers(t_game *g, int fd)
{
	int	found_identifiers;

	found_identifiers = 0;
	if (!parse_identifiers_loop(g, fd, &found_identifiers))
		return (0);
	return (found_identifiers == 6);
}
