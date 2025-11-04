/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:20:00 by diogribe          #+#    #+#             */
/*   Updated: 2025/11/04 22:34:58 by diogribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

// -----------------------------------------------------
// INPUT HANDLERS
// -----------------------------------------------------
int	key_press(int key, t_game *g)
{
	if (key == 65307)
		exit(0);
	if (key == 119)
		g->key_w = 1;
	if (key == 115)
		g->key_s = 1;
	if (key == 97)
		g->key_a = 1;
	if (key == 100)
		g->key_d = 1;
	return (0);
}

int	key_release(int key, t_game *g)
{
	if (key == 119)
		g->key_w = 0;
	if (key == 115)
		g->key_s = 0;
	if (key == 97)
		g->key_a = 0;
	if (key == 100)
		g->key_d = 0;
	return (0);
}

// -----------------------------------------------------
// MOVIMENTO - PRINCIPAL
// -----------------------------------------------------
void	update_movement(t_game *g)
{
	handle_player_move(g);
	handle_player_rotate(g);
}
