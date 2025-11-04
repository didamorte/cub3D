/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 20:45:00 by diogribe          #+#    #+#             */
/*   Updated: 2025/11/04 21:54:36 by diogribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

// -----------------------------------------------------
// ESCOLHER TEXTURA
// -----------------------------------------------------
int	select_texture(int side, double rayDirX, double rayDirY)
{
	int	texNum;

	texNum = 0;
	if (side == 0 && rayDirX > 0)
		texNum = 1;
	else if (side == 0 && rayDirX < 0)
		texNum = 0;
	else if (side == 1 && rayDirY > 0)
		texNum = 3;
	else if (side == 1 && rayDirY < 0)
		texNum = 2;
	return (texNum);
}

// -----------------------------------------------------
// CALCULAR COORDENADA X DA TEXTURA
// -----------------------------------------------------
int	calculate_tex_x(t_game *g, int side, double perpWallDist,
	double rayDirX, double rayDirY, int texNum)
{
	double	wallX;
	int		texX;

	if (side == 0)
		wallX = g->posY + perpWallDist * rayDirY;
	else
		wallX = g->posX + perpWallDist * rayDirX;
	wallX -= floor(wallX);
	texX = (int)(wallX * (double)g->textures[texNum].width);
	if ((side == 0 && rayDirX < 0) || (side == 1 && rayDirY > 0))
		texX = g->textures[texNum].width - texX - 1;
	if (texX < 0)
		texX = 0;
	if (texX >= g->textures[texNum].width)
		texX = g->textures[texNum].width - 1;
	return (texX);
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
	wall->tex_x = calculate_tex_x(g, ray->side, ray->perp_wall_dist,
			ray->dir_x, ray->dir_y, wall->tex_num);
}
