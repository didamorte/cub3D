/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:20:00 by diogribe          #+#    #+#             */
/*   Updated: 2026/01/22 00:41:52 by diogribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

#define MOVE_SPEED 0.01
#define ROT_SPEED 0.01

// -----------------------------------------------------
// POSIÇÃO DO PLAYER
// -----------------------------------------------------
void	set_player_direction(t_game *g, char c)
{
	g->dir_x = 0;
	g->dir_y = 0;
	g->plane_x = 0;
	g->plane_y = 0;
	if (c == 'N')
	{
		g->dir_y = -1;
		g->plane_x = 0.66;
	}
	else if (c == 'S')
	{
		g->dir_y = 1;
		g->plane_x = -0.66;
	}
	else if (c == 'E')
	{
		g->dir_x = 1;
		g->plane_y = 0.66;
	}
	else if (c == 'W')
	{
		g->dir_x = -1;
		g->plane_y = -0.66;
	}
}

void	find_player_start(t_game *g)
{
	int		y;
	int		x;
	char	c;

	y = 0;
	while (g->map[y])
	{
		x = 0;
		while (g->map[y][x])
		{
			c = g->map[y][x];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				g->pos_x = x + 0.5;
				g->pos_y = y + 0.5;
				set_player_direction(g, c);
				g->map[y][x] = '0';
				return ;
			}
			x++;
		}
		y++;
	}
	printf("Erro: posição inicial do jogador não encontrada!\n");
	close_game(g);
}

// -----------------------------------------------------
// MOVIMENTO - TRANSLAÇÃO
// -----------------------------------------------------
void	handle_player_move(t_game *g)
{
	move_ws(g);
	move_ad(g);
}

// -----------------------------------------------------
// MOVIMENTO - ROTAÇÃO
// -----------------------------------------------------
void	handle_player_rotate(t_game *g)
{
	double	old_dir_x;
	double	old_plane_x;
	double	angle;

	if (g->key_left)
		angle = -ROT_SPEED;
	else if (g->key_right)
		angle = ROT_SPEED;
	else
		return ;
	old_dir_x = g->dir_x;
	g->dir_x = g->dir_x * cos(angle) - g->dir_y * sin(angle);
	g->dir_y = old_dir_x * sin(angle) + g->dir_y * cos(angle);
	old_plane_x = g->plane_x;
	g->plane_x = g->plane_x * cos(angle) - g->plane_y * sin(angle);
	g->plane_y = old_plane_x * sin(angle) + g->plane_y * cos(angle);
}
