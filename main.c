/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 23:00:00 by diogribe          #+#    #+#             */
/*   Updated: 2026/01/20 12:50:10 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	main(int ac, char **av)
{
	t_game	g;

	if (!arg_check(ac, av))
		return (1);
	init_structs(&g);
	if (!parse_cub_file((&g), av[1]))
		return (1);
	find_player_start(&g);
	g.mlx = mlx_init();
	if (!g.mlx)
		return (1);
	g.win = mlx_new_window(g.mlx, WIDTH, HEIGHT, "cub3D");
	g.img = mlx_new_image(g.mlx, WIDTH, HEIGHT);
	g.addr = mlx_get_data_addr(g.img, &g.bpp, &g.line_length, &g.endian);
	create_textures(&g);
	mlx_hook(g.win, 2, 1L << 0, key_press, &g);
	mlx_hook(g.win, 3, 1L << 1, key_release, &g);
	mlx_hook(g.win, 17, 0, close_game, &g);
	mlx_loop_hook(g.mlx, loop_hook, &g);
	mlx_loop(g.mlx);
	return (0);
}
