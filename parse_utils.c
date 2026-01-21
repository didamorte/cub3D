/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 20:15:04 by nayara            #+#    #+#             */
/*   Updated: 2026/01/21 12:50:27 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	is_valid_rgb_value(char *s)
{
	int	i;

	i = 0;
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	if (!ft_isdigit(s[i]))
		return (0);
	while (ft_isdigit(s[i]))
		i++;
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	if (s[i] != '\0')
		return (0);
	return (1);
}

static int	has_invalid_commas(char *s)
{
	int	i;
	int	comma;

	i = 0;
	comma = 0;
	while (s[i])
	{
		if (s[i] == ',')
		{
			if (comma)
				return (1);
			comma = 1;
		}
		else if (ft_isdigit(s[i]))
			comma = 0;
		else if (s[i] != ' ' && s[i] != '\t')
			return (1);
		i++;
	}
	return (0);
}

static int	parse_rgb_values(char *line, int c[3])
{
	char	**rgb;
	int		i;

	if (has_invalid_commas(line))
		return (0);
	rgb = ft_split(line, ',');
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2] || rgb[3])
		return (free_matrix(rgb), 0);
	i = -1;
	while (++i < 3)
	{
		if (!is_valid_rgb_value(rgb[i]))
			return (free_matrix(rgb), 0);
		c[i] = ft_atoi(rgb[i]);
		if (c[i] < 0 || c[i] > 255)
			return (free_matrix(rgb), 0);
	}
	free_matrix(rgb);
	return (1);
}

int	save_color(t_game *g, t_type type, char *line)
{
	int	c[3];
	int	i;

	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	i++;
	if (!parse_rgb_values(line + i, c))
		return (0);
	if ((type == FLOOR && g->floor_color != -1)
		|| (type == CEILING && g->ceiling_color != -1))
		return (0);
	if (type == FLOOR)
		g->floor_color = (c[0] << 16 | c[1] << 8 | c[2]);
	else
		g->ceiling_color = (c[0] << 16 | c[1] << 8 | c[2]);
	return (1);
}

int	save_texture(t_game *g, int index, char *line)
{
	int	i;
	int	start;

	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	i += 2;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	start = i;
	while (line[i] && line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
		i++;
	if (g->tex_path[index])
		return (0);
	g->tex_path[index] = ft_substr(line, start, i - start);
	while (line[i] == ' ' || line[i] == '\t' || line[i] == '\r')
		i++;
	if (line[i] != '\0' && line[i] != '\n')
		return (free(g->tex_path[index]), g->tex_path[index] = NULL, 0);
	return (1);
}
