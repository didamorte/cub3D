/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 18:58:03 by nayara            #+#    #+#             */
/*   Updated: 2026/01/21 12:51:24 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	has_invalid_space_around(t_game *g, int y, int x)
{
	if (g->map[y][x + 1] == ' '
		|| g->map[y][x - 1] == ' '
		|| !g->map[y + 1] || g->map[y + 1][x] == ' '
		|| y == 0 || g->map[y - 1][x] == ' ')
		return (1);
	return (0);
}

static int	validate_map_char(t_game *g, int y, int x, int *p_count)
{
	if (g->map[y][x] == '\t' || g->map[y][x] == '\r')
		g->map[y][x] = ' ';
	if (!ft_strchr("01NSEW ", g->map[y][x]))
		return (1);
	if (ft_strchr("NSEW", g->map[y][x]))
		(*p_count)++;
	if ((g->map[y][x] == '0' || ft_strchr("NSEW", g->map[y][x]))
		&& has_invalid_space_around(g, y, x))
		return (1);
	return (0);
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
			if (validate_map_char(g, y, x, &p_count))
				return (1);
			x++;
		}
		y++;
	}
	if (p_count == 0)
		return (2);
	if (p_count > 1)
		return (3);
	return (0);
}
