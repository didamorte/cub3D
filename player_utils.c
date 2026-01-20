/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:36:46 by nayara            #+#    #+#             */
/*   Updated: 2026/01/20 13:41:29 by nayara           ###   ########.fr       */
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
		new_x = g->posX + g->dirX * MOVE_SPEED;
		new_y = g->posY + g->dirY * MOVE_SPEED;
		if (g->map[(int)g->posY][(int)new_x] == '0')
			g->posX = new_x;
		if (g->map[(int)new_y][(int)g->posX] == '0')
			g->posY = new_y;
	}
	if (g->key_s)
	{
		new_x = g->posX - g->dirX * MOVE_SPEED;
		new_y = g->posY - g->dirY * MOVE_SPEED;
		if (g->map[(int)g->posY][(int)new_x] == '0')
			g->posX = new_x;
		if (g->map[(int)new_y][(int)g->posX] == '0')
			g->posY = new_y;
	}
}

void	move_ad(t_game *g)
{
	double	new_x;
	double	new_y;

	if (g->key_a)
	{
		new_x = g->posX - g->dirY * MOVE_SPEED;
		new_y = g->posY + g->dirX * MOVE_SPEED;
		if (g->map[(int)g->posY][(int)new_x] == '0')
			g->posX = new_x;
		if (g->map[(int)new_y][(int)g->posX] == '0')
			g->posY = new_y;
	}
	if (g->key_d)
	{
		new_x = g->posX + g->dirY * MOVE_SPEED;
		new_y = g->posY - g->dirX * MOVE_SPEED;
		if (g->map[(int)g->posY][(int)new_x] == '0')
			g->posX = new_x;
		if (g->map[(int)new_y][(int)g->posX] == '0')
			g->posY = new_y;
	}
}
