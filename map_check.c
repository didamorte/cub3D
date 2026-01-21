/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 12:35:18 by nayara            #+#    #+#             */
/*   Updated: 2026/01/21 12:48:38 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	is_out_of_bounds(t_game *g, int y, int x)
{
	if (y == 0 || !g->map[y + 1] || x == 0 || g->map[y][x + 1] == '\0')
		return (1);
	if (x >= (int)ft_strlen(g->map[y - 1])
		|| x >= (int)ft_strlen(g->map[y + 1]))
		return (1);
	return (0);
}

static int	has_adjacent_space(t_game *g, int y, int x)
{
	if (g->map[y - 1][x] == ' ' || g->map[y][x - 1] == ' '
		|| g->map[y + 1][x] == ' ' || g->map[y][x + 1] == ' ')
		return (1);
	return (0);
}

int	is_map_closed(t_game *g)
{
	int	y;
	int	x;

	y = 0;
	while (g->map[y])
	{
		x = 0;
		while (g->map[y][x])
		{
			if (ft_strchr("0NSEW", g->map[y][x]))
			{
				if (is_out_of_bounds(g, y, x) || has_adjacent_space(g, y, x))
					return (0);
			}
			x++;
		}
		y++;
	}
	return (1);
}
