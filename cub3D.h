/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:38:06 by diogribe          #+#    #+#             */
/*   Updated: 2025/11/04 23:00:58 by diogribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DRAW_H
# define DRAW_H

# include "mlx.h"
# include <math.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>

# define WIDTH 800
# define HEIGHT 600

typedef struct s_texture
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_length;
	int		endian;
	int		width;
	int		height;
}	t_texture;

typedef struct s_ray
{
	double	dir_x;
	double	dir_y;
	int		map_x;
	int		map_y;
	int		step_x;
	int		step_y;
	double	side_dist_x;
	double	side_dist_y;
	int		side;
	double	perp_wall_dist;
}	t_ray;

typedef struct s_wall
{
	int		line_height;
	int		draw_start;
	int		draw_end;
	int		tex_num;
	int		tex_x;
}	t_wall;

typedef struct s_tex_draw
{
	double	step;
	double	tex_pos;
	int		tex_y;
	int		color;
}	t_tex_draw;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	void		*img;
	char		*addr;
	int			bpp;
	int			line_length;
	int			endian;

	char		**map;
	int			mapWidth;
	int			mapHeight;

	double		posX, posY;
	double		dirX, dirY;
	double		planeX, planeY;

	int			key_w;
	int			key_a;
	int			key_s;
	int			key_d;

	t_texture	textures[4];
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

int		key_press(int key, t_game *g);
int		key_release(int key, t_game *g);
void	update_movement(t_game *g);

#endif
