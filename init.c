/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:50:47 by nayara            #+#    #+#             */
/*   Updated: 2026/01/19 16:49:03 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	init_structs(t_game *g)
{
	int	i;

	// zera TUDO primeiro para evitar lixo de memória e erros de syscall
	ft_memset(g, 0, sizeof(t_game));
	// def valores especificos que não podem ser 0
	g->floor_color = -1;
	g->ceiling_color = -1;
	i = 0;
	while (i < 4)
	{
		g->tex_path[i] = NULL;
		i++;
	}
// o resto já foi zerado pelo memset (ponteiros a NULL e ints/doubles a 0)
}