/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cub_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 20:15:04 by nayara            #+#    #+#             */
/*   Updated: 2026/01/19 17:26:12 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	rgb_to_int(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}

int save_color(t_game *g, t_type type, char *line)
{
	char	**rgb;
	int		red;
	int		green;
	int		blue;

	// divide a string pelas vírgulas (ex: "220,100,0")
	rgb = ft_split(line, ',');
	if (!rgb || !rgb[1] || !rgb[2])
	{
		free_matrix(rgb);
		return (printf("Error\nInvalid color format\n"), 0);
	}
	red = ft_atoi((const char *)rgb);
	green = ft_atoi(rgb[1]);
	blue = ft_atoi(rgb[2]);
	// libera a matriz do split
	free_matrix(rgb); 
	// valida o intervalo de 0 a 255
	if (red < 0 || red > 255 || green < 0 || green > 255 || blue < 0 || blue > 255)
		return (printf("Error\nColor values must be between 0 and 255\n"), 0);
	// guarda a cor na struct para ser usada na clear_screen
	if (type == FLOOR)
		g->floor_color = rgb_to_int(red, green, blue);
	else if (type == CEILING)
		g->ceiling_color = rgb_to_int(red, green, blue);
	return (1);
}

int save_texture(t_game *g, int index, char *line)
{
	int	i;

	i = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	// guarda o path na struct para ser usado mais tarde
	g->tex_path[index] = ft_strdup(line + i);
	if (!g->tex_path[index])
		return (0);
	i = 0;
	/* while (g->tex_path[index][i] && g->tex_path[index][i] != '\n')
		i++; */
	while (g->tex_path[index][i])
	{
		// Substitui o final da linha (espaço, tab ou newline) por \0
		if (g->tex_path[index][i] == '\n' || g->tex_path[index][i] == ' '
			|| g->tex_path[index][i] == '\t' || g->tex_path[index][i] == '\r')
		{
			g->tex_path[index][i] = '\0';
			break ;
		}
		i++;
	}
	g->tex_path[index][i] = '\0'; // remove o \n final
	return (1);
}


int is_empty_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
			return (0); // encontrou algo q nao é espaço == nao esta vazia
		i++;
	}
	return (1); // só tinha espaços == vazia
}

int	process_line(t_game *g, char *line)
{
	if (!ft_strncmp(line, "NO ", 3) && !g->textures[NO].img)
		return (save_texture(g, NO, line + 3));
	if (!ft_strncmp(line, "SO ", 3) && !g->textures[SO].img)
		return (save_texture(g, SO, line + 3));
	if (!ft_strncmp(line, "WE ", 3) && !g->textures[WE].img)
		return (save_texture(g, WE, line + 3));
	if (!ft_strncmp(line, "EA ", 3) && !g->textures[EA].img)
		return (save_texture(g, EA, line + 3));
	if (!ft_strncmp(line, "F ", 2) && g->floor_color == -1)
		return (save_color(g, FLOOR, line + 2));
	if (!ft_strncmp(line, "C ", 2) && g->ceiling_color == -1)
		return (save_color(g, CEILING, line + 2));
	return (0);
}
