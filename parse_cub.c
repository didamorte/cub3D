/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 19:09:59 by nayara            #+#    #+#             */
/*   Updated: 2026/01/19 17:27:25 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

// extrai caminhos de texturas e cores RGB
int parse_identifiers(t_game *g, int fd)
{
	char	*line;
	int	found_elements;

	found_elements = 0;
	while (found_elements < 6)
	{
		line = get_next_line(fd);
		if (!line)
			break;
		if (is_empty_line(line))
		{
			free(line);
			continue;
		}
		if (process_line(g, line))
			found_elements++;
		else
		{
			free(line);
			return (printf("Error\nInvalid configuration element\n"), 0);
		}
		free(line);
	}
	return (found_elements == 6);
}

// verifica se os caracteres são válidos (0, 1, N, S, E, W)
int check_map_chars(t_game *g)
{
	int	y;
	int	x;
	int	p_count;

	y = 0;
	p_count = 0;
	while (g->map[y]) // percorre as linhas
	{
		x = 0;
		while (g->map[y][x]) // percorre os chars da linha
		{
			if (!ft_strchr("01NSEW", g->map[y][x])) // se o char é autorizado ou espaço
				return (0);
			if (ft_strchr("NSEW", g->map[y][x])) // se for um caractere de jogador, incremeta o pcount
				p_count++;
			x++;
		}
		y++;
	}
	return (p_count == 1); // o subject exige exatamente uma posição inicial
}

// verifica se está cercado por paredes
int is_map_closed(t_game *g)
{
	int	x;
	int y;

	y = 0;
	while (g->map[y])
	{
		x = 0;
		while (g->map[y][x])
		{
			// se encontrarmos o chao (0) ou o jogador (NSEW)
			if (ft_strchr("0NSEW", g->map[y][x]))
			{
				// checar se esta nos limites extremos da matriz
				if (y == 0 || !g->map[y + 1] || x == 0 || g->map[y][x + 1] == '\0')
					return (0);
				//verificar cima baixo esquerda e direita
				// se algum destes for espaço ou NULL, o mapa esta aberto
				if (g->map[y - 1][x] == ' ' || g->map[y][x - 1] == ' ' 
					|| g->map[y + 1][x] == ' ' || g->map[y][x + 1] == ' ')
					return (0);
				// checagem para linhas de tamanhos diferentes
				if (x >= (int)ft_strlen(g->map[y - 1]) 
					|| x >= (int)ft_strlen(g->map[y + 1]))
					return (0);
			}
			x++;
		}
		y++;
	}
	return (1);
}

char	*read_map_to_string(int fd)
{
	char	*line;
	char	*tmp;
	char	*full_str;

	full_str = ft_strdup("");
	// pular linhas vazias antes do mapa
	line = get_next_line(fd);
	while (line && is_empty_line(line))
	{
		free(line);
		line = get_next_line(fd);
	}
	// ler as linhas do mapa e concatenar
	while (line)
	{
		tmp = full_str;
		full_str = ft_strjoin(tmp, line);
		free(tmp);
		free(line);
		line = get_next_line(fd);
	}
	if (ft_strlen(full_str) == 0)
		return (free(full_str), NULL);
	return (full_str);
}

int parse_cub_file(t_game *g, char *file_path)
{
	int fd;
	char	*map_buffer;

	fd = open(file_path, O_RDONLY);
	if (fd < 0)
		return (printf("Error\nCould not open file\n"), 0);
	// 1 - ler texturas e cores
	if (!parse_identifiers(g, fd))
	{
		close(fd);
		return (0);
	}
	// 2 - ler o mapa para g->map
	map_buffer = read_map_to_string(fd);
	if (!map_buffer)
		return (printf("Error\nMap is missing\n"));
	// transofrma a string num array de strings 
	g->map = ft_split(map_buffer, '\n');
	free(map_buffer);
	close(fd);
	// 3 - validar
	if (!check_map_chars(g))
		return (printf("Error\nInvalid characters or multiple players\n"), 0);
	if (!is_map_closed(g))
		return (printf("Error\nMap is not closed by walls\n"), 0);
	return (1);
}