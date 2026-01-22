/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:36:46 by nayara            #+#    #+#             */
/*   Updated: 2026/01/22 00:54:29 by diogribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

#define MOVE_SPEED 0.01
#define ROT_SPEED 0.01

void	move_ws(t_game *g)
{
	double	new_x;
	double	new_y;

	if (g->key_w)
	{
		new_x = g->pos_x + g->dir_x * MOVE_SPEED;
		new_y = g->pos_y + g->dir_y * MOVE_SPEED;
		if (g->map[(int)g->pos_y][(int)new_x] == '0')
			g->pos_x = new_x;
		if (g->map[(int)new_y][(int)g->pos_x] == '0')
			g->pos_y = new_y;
	}
	if (g->key_s)
	{
		new_x = g->pos_x - g->dir_x * MOVE_SPEED;
		new_y = g->pos_y - g->dir_y * MOVE_SPEED;
		if (g->map[(int)g->pos_y][(int)new_x] == '0')
			g->pos_x = new_x;
		if (g->map[(int)new_y][(int)g->pos_x] == '0')
			g->pos_y = new_y;
	}
}

void	move_ad(t_game *g)
{
	double	new_x;
	double	new_y;

	if (g->key_a)
	{
		new_x = g->pos_x + g->dir_y * MOVE_SPEED;
		new_y = g->pos_y - g->dir_x * MOVE_SPEED;
		if (g->map[(int)g->pos_y][(int)new_x] == '0')
			g->pos_x = new_x;
		if (g->map[(int)new_y][(int)g->pos_x] == '0')
			g->pos_y = new_y;
	}
	if (g->key_d)
	{
		new_x = g->pos_x - g->dir_y * MOVE_SPEED;
		new_y = g->pos_y + g->dir_x * MOVE_SPEED;
		if (g->map[(int)g->pos_y][(int)new_x] == '0')
			g->pos_x = new_x;
		if (g->map[(int)new_y][(int)g->pos_x] == '0')
			g->pos_y = new_y;
	}
}
