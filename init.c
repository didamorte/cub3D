/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:50:47 by nayara            #+#    #+#             */
/*   Updated: 2026/01/20 13:40:17 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	init_structs(t_game *g)
{
	int	i;

	ft_memset(g, 0, sizeof(t_game));
	g->floor_color = -1;
	g->ceiling_color = -1;
	i = 0;
	while (i < 4)
	{
		g->tex_path[i] = NULL;
		i++;
	}
}
