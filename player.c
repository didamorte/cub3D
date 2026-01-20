/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:20:00 by diogribe          #+#    #+#             */
/*   Updated: 2026/01/20 13:41:35 by nayara           ###   ########.fr       */
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
	g->dirX = 0;
	g->dirY = 0;
	g->planeX = 0;
	g->planeY = 0;
	if (c == 'N')
	{
		g->dirY = -1;
		g->planeX = 0.66;
	}
	else if (c == 'S')
	{
		g->dirY = 1;
		g->planeX = -0.66;
	}
	else if (c == 'E')
	{
		g->dirX = 1;
		g->planeY = 0.66;
	}
	else if (c == 'W')
	{
		g->dirX = -1;
		g->planeY = -0.66;
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
				g->posX = x + 0.5;
				g->posY = y + 0.5;
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
	old_dir_x = g->dirX;
	g->dirX = g->dirX * cos(angle) - g->dirY * sin(angle);
	g->dirY = old_dir_x * sin(angle) + g->dirY * cos(angle);
	old_plane_x = g->planeX;
	g->planeX = g->planeX * cos(angle) - g->planeY * sin(angle);
	g->planeY = old_plane_x * sin(angle) + g->planeY * cos(angle);
}
