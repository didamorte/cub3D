/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cub_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 20:15:04 by nayara            #+#    #+#             */
/*   Updated: 2026/01/20 13:47:00 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	rgb_to_int(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}

static char	*trim_crlf(char *s)
{
	int	len;

	len = ft_strlen(s);
	if (len > 0 && s[len - 1] == '\r')
		s[len - 1] = '\0';
	return (s);
}

int	save_color(t_game *g, t_type type, char *line)
{
	char	**rgb;
	int		red;
	int		green;
	int		blue;

	line = trim_crlf(line);
	rgb = ft_split(line, ',');
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2])
	{
		free_matrix(rgb);
		return (printf("Error\nInvalid color format\n"), 0);
	}
	red = ft_atoi(rgb[0]);
	green = ft_atoi(rgb[1]);
	blue = ft_atoi(rgb[2]);
	free_matrix(rgb);
	if (red < 0 || red > 255 || green < 0 || green > 255
		|| blue < 0 || blue > 255)
		return (printf("Error\nColor values must be between 0 and 255\n"), 0);
	if (type == FLOOR)
		g->floor_color = rgb_to_int(red, green, blue);
	else if (type == CEILING)
		g->ceiling_color = rgb_to_int(red, green, blue);
	return (1);
}

int	save_texture(t_game *g, int index, char *line)
{
	int	i;

	i = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	g->tex_path[index] = ft_strdup(line + i);
	if (!g->tex_path[index])
		return (0);
	i = 0;
	while (g->tex_path[index][i])
	{
		if (g->tex_path[index][i] == '\n' || g->tex_path[index][i] == ' '
			|| g->tex_path[index][i] == '\t' || g->tex_path[index][i] == '\r')
		{
			g->tex_path[index][i] = '\0';
			break ;
		}
		i++;
	}
	g->tex_path[index][i] = '\0';
	return (1);
}
