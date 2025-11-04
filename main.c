/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 23:00:00 by diogribe          #+#    #+#             */
/*   Updated: 2025/11/04 23:05:05 by diogribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static char	*g_map[] = {
	"111111",
	"100001",
	"10N101",
	"100001",
	"111111",
	NULL
};

int	main(void)
{
	t_game	g;

	g.map = dup_map(g_map);
	find_player_start(&g);
	g.key_w = 0;
	g.key_a = 0;
	g.key_s = 0;
	g.key_d = 0;
	g.mlx = mlx_init();
	g.win = mlx_new_window(g.mlx, WIDTH, HEIGHT, "Raycasting - Linux");
	g.img = mlx_new_image(g.mlx, WIDTH, HEIGHT);
	g.addr = mlx_get_data_addr(g.img, &g.bpp, &g.line_length, &g.endian);
	create_textures(&g);
	mlx_hook(g.win, 2, 1L << 0, key_press, &g);
	mlx_hook(g.win, 3, 1L << 1, key_release, &g);
	mlx_hook(g.win, 17, 0, close_window, &g);
	mlx_loop_hook(g.mlx, loop_hook, &g);
	mlx_loop(g.mlx);
	return (0);
}
