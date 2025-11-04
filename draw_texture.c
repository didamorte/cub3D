/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 20:45:00 by diogribe          #+#    #+#             */
/*   Updated: 2025/11/04 22:51:09 by diogribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

// -----------------------------------------------------
// ESCOLHER TEXTURA
// -----------------------------------------------------
int	select_texture(int side, double rayDirX, double rayDirY)
{
	int	tex_num;

	tex_num = 0;
	if (side == 0 && rayDirX > 0)
		tex_num = 1;
	else if (side == 0 && rayDirX < 0)
		tex_num = 0;
	else if (side == 1 && rayDirY > 0)
		tex_num = 3;
	else if (side == 1 && rayDirY < 0)
		tex_num = 2;
	return (tex_num);
}

// -----------------------------------------------------
// CALCULAR COORDENADA X DA TEXTURA
// -----------------------------------------------------
int	calculate_tex_x(t_game *g, t_ray *ray, int tex_num)
{
	double	wall_x;
	int		tex_x;

	if (ray->side == 0)
		wall_x = g->posY + ray->perp_wall_dist * ray->dir_y;
	else
		wall_x = g->posX + ray->perp_wall_dist * ray->dir_x;
	wall_x -= floor(wall_x);
	tex_x = (int)(wall_x * (double)g->textures[tex_num].width);
	if ((ray->side == 0 && ray->dir_x < 0)
		|| (ray->side == 1 && ray->dir_y > 0))
		tex_x = g->textures[tex_num].width - tex_x - 1;
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= g->textures[tex_num].width)
		tex_x = g->textures[tex_num].width - 1;
	return (tex_x);
}

// -----------------------------------------------------
// DESENHAR COLUNA TEXTURIZADA
// -----------------------------------------------------
void	draw_textured_column(t_game *g, int x, t_wall *w)
{
	t_tex_draw	td;
	int			y;

	init_tex_draw(&td, w, g->textures[w->tex_num].height);
	y = w->draw_start;
	while (y <= w->draw_end)
	{
		td.tex_y = (int)td.tex_pos;
		if (td.tex_y >= g->textures[w->tex_num].height)
			td.tex_y = g->textures[w->tex_num].height - 1;
		else if (td.tex_y < 0)
			td.tex_y = 0;
		td.color = get_texture_color(g, w, td.tex_y);
		my_mlx_pixel_put(g, x, y, td.color);
		td.tex_pos += td.step;
		y++;
	}
}

// -----------------------------------------------------
// CALCULAR WALL STRUCT
// -----------------------------------------------------
void	calc_wall(t_game *g, t_ray *ray, t_wall *wall)
{
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - g->posX
				+ (1 - ray->step_x) / 2) / ray->dir_x;
	else
		ray->perp_wall_dist = (ray->map_y - g->posY
				+ (1 - ray->step_y) / 2) / ray->dir_y;
	wall->line_height = (int)(HEIGHT / ray->perp_wall_dist);
	wall->draw_start = -wall->line_height / 2 + HEIGHT / 2;
	wall->draw_end = wall->line_height / 2 + HEIGHT / 2;
	wall->tex_num = select_texture(ray->side, ray->dir_x, ray->dir_y);
	wall->tex_x = calculate_tex_x(g, ray, wall->tex_num);
}
