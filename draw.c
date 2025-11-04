/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:41:13 by diogribe          #+#    #+#             */
/*   Updated: 2025/11/04 21:54:28 by diogribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

// -----------------------------------------------------
// INICIALIZAR RAY STRUCT
// -----------------------------------------------------
void	init_ray_struct(t_game *g, int x, t_ray *ray)
{
	double	camera_x;

	camera_x = 2 * x / (double)WIDTH - 1;
	ray->dir_x = g->dirX + g->planeX * camera_x;
	ray->dir_y = g->dirY + g->planeY * camera_x;
	ray->map_x = (int)g->posX;
	ray->map_y = (int)g->posY;
}

// -----------------------------------------------------
// CALCULAR STEP E SIDE DIST PARA RAY
// -----------------------------------------------------
void	calc_ray_step(t_game *g, t_ray *ray)
{
	double	delta_x;
	double	delta_y;

	delta_x = fabs(1 / ray->dir_x);
	delta_y = fabs(1 / ray->dir_y);
	if (ray->dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (g->posX - ray->map_x) * delta_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - g->posX) * delta_x;
	}
	if (ray->dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (g->posY - ray->map_y) * delta_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - g->posY) * delta_y;
	}
}

// -----------------------------------------------------
// DDA COM RAY STRUCT
// -----------------------------------------------------
void	perform_dda_ray(t_game *g, t_ray *ray)
{
	int		hit;
	double	delta_x;
	double	delta_y;

	hit = 0;
	delta_x = fabs(1 / ray->dir_x);
	delta_y = fabs(1 / ray->dir_y);
	while (hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += delta_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += delta_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (g->map[ray->map_y][ray->map_x] == '1')
			hit = 1;
	}
}

// -----------------------------------------------------
// PROCESSAR UM RAY (COLUNA)
// -----------------------------------------------------
void	cast_ray(t_game *g, int x)
{
	t_ray	ray;
	t_wall	wall;

	init_ray_struct(g, x, &ray);
	calc_ray_step(g, &ray);
	perform_dda_ray(g, &ray);
	calc_wall(g, &ray, &wall);
	draw_textured_column(g, x, &wall);
}

// -----------------------------------------------------
// RAYCASTING PRINCIPAL
// -----------------------------------------------------
void	draw_frame(t_game *g)
{
	int	x;

	clear_screen(g);
	x = 0;
	while (x < WIDTH)
	{
		cast_ray(g, x);
		x++;
	}
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
}
