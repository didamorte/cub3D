/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 20:58:27 by nayara            #+#    #+#             */
/*   Updated: 2026/01/19 16:47:49 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	free_matrix(char **matrix)
{
	int	i;

	i = 0;
	// se a matriz já for nula não faz nada
	if (!matrix)
		return ;
	// percorre a matriz liberando cada linha individualmente
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	// libera o ponteiro que guardava o array de ponteiros
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

int	close_game(t_game *g)
{
	int	i;

	// Liberar a matriz do mapa
	if (g->map)
		free_matrix(g->map);
	// liberar os caminhos das texturas
	i = 0;
	while (i < 4)
	{
		if (g->tex_path[i])
			free(g->tex_path[i]);
		i++;
	}
	// liberar as imagens das texturas da MLX
	i = 0;
	while (i < 4)
	{
		if (g->textures[i].img)
			mlx_destroy_image(g->mlx, g->textures[i].img);
		i++;
	}
	if (g->img)
		mlx_destroy_image(g->mlx, g->img);
	// destruir a janela e a instância MLX
	if (g->win)
		mlx_destroy_window(g->mlx, g->win);
	mlx_destroy_display(g->mlx);
	free(g->mlx);
	printf("Clean exit successful!\n");
	exit(0);
	return (0);
}