/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launcher.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 23:00:00 by diogribe          #+#    #+#             */
/*   Updated: 2025/11/04 23:04:57 by diogribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

// -----------------------------------------------------
// DUPLICA MAPA
// -----------------------------------------------------
char	**dup_map(char **src)
{
	int		i;
	int		len;
	char	**dst;

	len = 0;
	while (src[len])
		len++;
	dst = malloc((len + 1) * sizeof(char *));
	i = 0;
	while (i < len)
	{
		dst[i] = strdup(src[i]);
		i++;
	}
	dst[i] = NULL;
	return (dst);
}

// -----------------------------------------------------
// CARREGAR TEXTURAS XPM
// -----------------------------------------------------
void	load_texture(t_game *g, int index, char *path)
{
	g->textures[index].img = mlx_xpm_file_to_image(g->mlx, path,
			&g->textures[index].width, &g->textures[index].height);
	if (!g->textures[index].img)
	{
		fprintf(stderr, "Erro: não foi possível carregar %s\n", path);
		exit(1);
	}
	g->textures[index].addr = mlx_get_data_addr(g->textures[index].img,
			&g->textures[index].bpp, &g->textures[index].line_length,
			&g->textures[index].endian);
}

void	create_textures(t_game *g)
{
	load_texture(g, 0, "textures/east.xpm");
	load_texture(g, 1, "textures/west.xpm");
	load_texture(g, 2, "textures/south.xpm");
	load_texture(g, 3, "textures/north.xpm");
}

// -----------------------------------------------------
// FECHAR JANELA (X)
// -----------------------------------------------------
int	close_window(t_game *g)
{
	(void)g;
	exit(0);
	return (0);
}

// -----------------------------------------------------
// LOOP PRINCIPAL
// -----------------------------------------------------
int	loop_hook(t_game *g)
{
	update_movement(g);
	draw_frame(g);
	return (0);
}
