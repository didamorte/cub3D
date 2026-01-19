/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:38:06 by diogribe          #+#    #+#             */
/*   Updated: 2026/01/17 18:20:16 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DRAW_H
# define DRAW_H

# include "minilibx-linux/mlx.h"
# include "libft/libft.h"
# include <math.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <unistd.h>

# define WIDTH 800
# define HEIGHT 600

typedef enum e_type
{
	NO,	/* 0 - Textura Norte */
	SO,	/* 1 - Textura Sul */
	WE,	/* 2 - Textura Oeste */
	EA,	/* 3 - Textura Leste */
	FLOOR,	/* 4 - Cor do Chão (F) */
	CEILING	/* 5 - Cor do Teto (C) */
}	t_type;

typedef struct s_texture
{
	void	*img; // ponteiro para a imagem carregada mlx_xpm_file_to_image
	char	*addr; // variaveis fundamentais para manipulação de imgs para minilibx
	int		bpp; // permitem ao código ler a cor de cada pixel da textura
	int		line_length;
	int		endian;
	int		width; //O tamanho da imagem (ex: 64x64 pixels)
	int		height; // O código usa isto para saber como esticar a imagem na parede
}	t_texture;

typedef struct s_ray
{
	double	dir_x; // A direção de um laser específico que está a ser disparado
	double	dir_y; // " "
	int		map_x; // Em que quadrado da grelha do mapa o laser está no momento
	int		map_y; // " "
	int		step_x; // ndica se o laser se move para a esquerda/direita ou cima/baixo (-1 ou 1)
	int		step_y; // " "
	double	side_dist_x; // A distância que o laser já percorreu até atingir a próxima linha da grelha
	double	side_dist_y; // 
	int		side; // Diz se o laser bateu numa parede vertical (Norte/Sul) ou horizontal (Este/Oeste)
	double	perp_wall_dist; // A distância real e perpendicular entre o jogador e a parede. 
						// É usada para calcular a altura da parede no ecrã sem o efeito de "olho de peixe"
}	t_ray;

typedef struct s_wall
{
	int		line_height; //  Quantos pixels de altura a parede terá no ecrã (quanto mais perto, maior)
	int		draw_start; // Onde começa (topo) e termina (base) o desenho da parede na coluna vertical da janela
	int		draw_end; // " "
	int		tex_num; // Qual das 4 texturas será usada para esta parede específica
	int		tex_x; // Qual coluna horizontal da textura deve ser desenhada (para que a imagem não fique apenas de uma cor)
}	t_wall;

typedef struct s_tex_draw
{
	double	step; // Quanto devemos saltar na textura para cada pixel desenhado no ecrã (ajusta a escala da imagem)
	double	tex_pos; // A posição atual dentro da textura, em formato decimal
	int		tex_y; // A coordenada inteira do pixel na textura que vamos copiar
	int		color; // A cor final do pixel extraída da textura para ser colocada no buffer
}	t_tex_draw;

typedef struct s_game
{
	void		*mlx; // ponteiro para minilibs
	void		*win; // ponteiro para a janela aberta

	/* dados tecnicos de manipulaçao de img pela minilibx */
	void		*img; 
	char		*addr;
	int			bpp;
	int			line_length;
	int			endian;

	char		**map; // matriz char que guardará o layout do mapa lido pelo parser
	int			mapWidth; // largura do mapa 
	int			mapHeight; // altura do mapa

	double		posX, posY; // posição exata do jogador no mapa
	double		dirX, dirY; // vetor q idica para onde o jogador esta a olhar diretamente
	double		planeX, planeY; // plano da 'camera' q define o campo de visao do player

	int			key_w; // flags que dizem se a tecla esta permitida (1) ou nao (0)
	int			key_a;
	int			key_s;
	int			key_d;
	int			key_left;
	int			key_right;

	/* Novas variáveis para as cores do ambiente */
	int			floor_color; /* Guarda a cor do chão (F) em hexadecimal */
	int			ceiling_color; /* Guarda a cor do teto (C) em hexadecimal */

	t_texture	textures[4]; // array q guarda as quatro texturas (norte, sul, leste e oeste)
	char	*tex_path[4]; // guarda os caminhos para NO SO WE EA
}	t_game;

void	my_mlx_pixel_put(t_game *game, int x, int y, int color);
void	draw_frame(t_game *g);

void	clear_screen(t_game *g);
int		get_texture_color(t_game *g, t_wall *w, int tex_y);
void	init_tex_draw(t_tex_draw *td, t_wall *w, int tex_height);

int		select_texture(int side, double rayDirX, double rayDirY);
int		calculate_tex_x(t_game *g, t_ray *ray, int tex_num);
void	draw_textured_column(t_game *g, int x, t_wall *w);
void	calc_wall(t_game *g, t_ray *ray, t_wall *wall);

void	init_ray_struct(t_game *g, int x, t_ray *ray);
void	calc_ray_step(t_game *g, t_ray *ray);
void	perform_dda_ray(t_game *g, t_ray *ray);
void	cast_ray(t_game *g, int x);

char	**dup_map(char **src);
void	load_texture(t_game *g, int index, char *path);
void	create_textures(t_game *g);
int		close_window(t_game *g);
int		loop_hook(t_game *g);

void	set_player_direction(t_game *g, char c);
void	find_player_start(t_game *g);
void	handle_player_move(t_game *g);
void	handle_player_rotate(t_game *g);
void	move_ad(t_game *g);
void	move_ws(t_game *g);

int		key_press(int key, t_game *g);
int		key_release(int key, t_game *g);
void	update_movement(t_game *g);

int	arg_check(int ac, char **av);
void	free_textures_paths(t_game *g);
int	parse_identifiers(t_game *g, int fd);
int	check_map_chars(t_game *g);
int	is_map_closed(t_game *g);
char	*read_map_to_string(int fd);
int	parse_cub_file(t_game *g, char *file_path);
void	free_matrix(char **matrix);
int	save_color(t_game *g, t_type type, char *line);
int	save_texture(t_game *g, int index, char *line);
int	is_empty_line(char *line);
int	process_line(t_game *g, char *line);
int	close_game(t_game *g);
void	init_structs(t_game *g);

#endif
