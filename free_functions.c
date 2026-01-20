/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 20:58:27 by nayara            #+#    #+#             */
/*   Updated: 2026/01/20 13:49:28 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	free_matrix(char **matrix)
{
	int	i;

	i = 0;
	if (!matrix)
		return ;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}

void	free_textures_paths(t_game *g)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (g->tex_path[i])
			free(g->tex_path[i]);
		i++;
	}
}

static void	free_paths_and_textures(t_game *g)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (g->tex_path[i])
			free(g->tex_path[i]);
		if (g->textures[i].img)
			mlx_destroy_image(g->mlx, g->textures[i].img);
		i++;
	}
}

static void	destroy_mlx_resources(t_game *g)
{
	if (g->img)
		mlx_destroy_image(g->mlx, g->img);
	if (g->win)
		mlx_destroy_window(g->mlx, g->win);
	mlx_destroy_display(g->mlx);
	free(g->mlx);
}

int	close_game(t_game *g)
{
	if (g->map)
		free_matrix(g->map);
	free_paths_and_textures(g);
	destroy_mlx_resources(g);
	printf("Clean exit successful!\n");
	exit(0);
	return (0);
}
