/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:41:13 by diogribe          #+#    #+#             */
/*   Updated: 2025/10/13 19:04:24 by diogribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

// -----------------------------------------------------
// PIXEL
// -----------------------------------------------------
void	my_mlx_pixel_put(t_game *game, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	dst = game->addr + (y * game->line_length + x * (game->bpp / 8));
	*(unsigned int *)dst = color;
}

// -----------------------------------------------------
// DESENHAR COLUNA
// -----------------------------------------------------
void	draw_vertical_line(t_game *game, int x, int start, int end, int color)
{
	int	y;

	if (start < 0)
		start = 0;
	if (end >= HEIGHT)
		end = HEIGHT - 1;
	y = start;
	while (y <= end)
	{
		my_mlx_pixel_put(game, x, y, color);
		y++;
	}
}

// -----------------------------------------------------
// LIMPAR TELA (CÉU + CHÃO)
// -----------------------------------------------------
static void	clear_screen(t_game *g)
{
	int	x;
	int	y;
	int	color;

	y = 0;
	while (y < HEIGHT)
	{
		color = (y < HEIGHT / 2) ? 0x87CEEB : 0x444444;
		x = 0;
		while (x < WIDTH)
		{
			my_mlx_pixel_put(g, x, y, color);
			x++;
		}
		y++;
	}
}

// -----------------------------------------------------
// INICIALIZAR RAY
// -----------------------------------------------------
static void	init_ray(t_game *g, int x, double *rayDirX, double *rayDirY,
	int *mapX, int *mapY)
{
	double	cameraX;

	cameraX = 2 * x / (double)WIDTH - 1;
	*rayDirX = g->dirX + g->planeX * cameraX;
	*rayDirY = g->dirY + g->planeY * cameraX;
	*mapX = (int)g->posX;
	*mapY = (int)g->posY;
}

// -----------------------------------------------------
// CALCULAR STEP E SIDE DIST
// -----------------------------------------------------
static void	calculate_step_and_sidedist(t_game *g, double rayDirX,
	double rayDirY, int *stepX, int *stepY, double *sideDistX,
	double *sideDistY, int mapX, int mapY)
{
	double	deltaDistX;
	double	deltaDistY;

	deltaDistX = fabs(1 / rayDirX);
	deltaDistY = fabs(1 / rayDirY);
	if (rayDirX < 0)
	{
		*stepX = -1;
		*sideDistX = (g->posX - mapX) * deltaDistX;
	}
	else
	{
		*stepX = 1;
		*sideDistX = (mapX + 1.0 - g->posX) * deltaDistX;
	}
	if (rayDirY < 0)
	{
		*stepY = -1;
		*sideDistY = (g->posY - mapY) * deltaDistY;
	}
	else
	{
		*stepY = 1;
		*sideDistY = (mapY + 1.0 - g->posY) * deltaDistY;
	}
}

// -----------------------------------------------------
// ALGORITMO DDA - ENCONTRAR PAREDE
// -----------------------------------------------------
static void	perform_dda(t_game *g, int *mapX, int *mapY, double *sideDistX,
	double *sideDistY, int stepX, int stepY, int *side, double rayDirX,
	double rayDirY)
{
	int		hit;
	double	deltaDistX;
	double	deltaDistY;

	hit = 0;
	deltaDistX = fabs(1 / rayDirX);
	deltaDistY = fabs(1 / rayDirY);
	while (hit == 0)
	{
		if (*sideDistX < *sideDistY)
		{
			*sideDistX += deltaDistX;
			*mapX += stepX;
			*side = 0;
		}
		else
		{
			*sideDistY += deltaDistY;
			*mapY += stepY;
			*side = 1;
		}
		if (g->map[*mapY][*mapX] == '1')
			hit = 1;
	}
}

// -----------------------------------------------------
// CALCULAR DISTÂNCIA E ALTURA DA PAREDE
// -----------------------------------------------------
static void	calculate_wall_height(t_game *g, int mapX, int mapY, int side,
	double rayDirX, double rayDirY, int stepX, int stepY,
	int *lineHeight, int *drawStart, int *drawEnd)
{
	double	perpWallDist;

	if (side == 0)
		perpWallDist = (mapX - g->posX + (1 - stepX) / 2) / rayDirX;
	else
		perpWallDist = (mapY - g->posY + (1 - stepY) / 2) / rayDirY;
	*lineHeight = (int)(HEIGHT / perpWallDist);
	*drawStart = -*lineHeight / 2 + HEIGHT / 2;
	*drawEnd = *lineHeight / 2 + HEIGHT / 2;
}

// -----------------------------------------------------
// ESCOLHER TEXTURA
// -----------------------------------------------------
static int	select_texture(int side, double rayDirX, double rayDirY)
{
	int	texNum;

	texNum = 0;
	if (side == 0 && rayDirX > 0)
		texNum = 1;  // Raio vai Este → parede virada Oeste
	else if (side == 0 && rayDirX < 0)
		texNum = 0;  // Raio vai Oeste → parede virada Este
	else if (side == 1 && rayDirY > 0)
		texNum = 3;  // Raio vai Sul → parede virada Norte
	else if (side == 1 && rayDirY < 0)
		texNum = 2;  // Raio vai Norte → parede virada Sul
	return (texNum);
}

// -----------------------------------------------------
// CALCULAR COORDENADA X DA TEXTURA
// -----------------------------------------------------
static int	calculate_tex_x(t_game *g, int side, double perpWallDist,
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
static void	draw_textured_column(t_game *g, int x, int drawStart, int drawEnd,
	int lineHeight, int texNum, int texX)
{
	t_texture	*tex;
	int			tex_bpp_div;
	double		step;
	double		texPos;
	int			y;
	int			texY;
	int			color;

	step = 1.0 * g->textures[texNum].height / lineHeight;
	texPos = (drawStart - HEIGHT / 2 + lineHeight / 2) * step;
	if (drawStart < 0)
	{
		texPos += step * (-drawStart);
		drawStart = 0;
	}
	if (drawEnd >= HEIGHT)
		drawEnd = HEIGHT - 1;
	tex = &g->textures[texNum];
	tex_bpp_div = tex->bpp / 8;
	y = drawStart;
	while (y <= drawEnd)
	{
		texY = (int)texPos;
		if (texY >= tex->height)
			texY = tex->height - 1;
		else if (texY < 0)
			texY = 0;
		color = *(unsigned int *)(tex->addr
				+ (texY * tex->line_length + texX * tex_bpp_div));
		my_mlx_pixel_put(g, x, y, color);
		texPos += step;
		y++;
	}
}

// -----------------------------------------------------
// PROCESSAR UM RAY (COLUNA)
// -----------------------------------------------------
static void	cast_ray(t_game *g, int x)
{
	double	rayDirX;
	double	rayDirY;
	int		mapX;
	int		mapY;
	int		stepX;
	int		stepY;
	double	sideDistX;
	double	sideDistY;
	int		side;
	int		lineHeight;
	int		drawStart;
	int		drawEnd;
	int		texNum;
	int		texX;
	double	perpWallDist;

	init_ray(g, x, &rayDirX, &rayDirY, &mapX, &mapY);
	calculate_step_and_sidedist(g, rayDirX, rayDirY, &stepX, &stepY,
		&sideDistX, &sideDistY, mapX, mapY);
	perform_dda(g, &mapX, &mapY, &sideDistX, &sideDistY, stepX, stepY,
		&side, rayDirX, rayDirY);
	calculate_wall_height(g, mapX, mapY, side, rayDirX, rayDirY, stepX,
		stepY, &lineHeight, &drawStart, &drawEnd);
	texNum = select_texture(side, rayDirX, rayDirY);
	if (side == 0)
		perpWallDist = (mapX - g->posX + (1 - stepX) / 2) / rayDirX;
	else
		perpWallDist = (mapY - g->posY + (1 - stepY) / 2) / rayDirY;
	texX = calculate_tex_x(g, side, perpWallDist, rayDirX, rayDirY, texNum);
	draw_textured_column(g, x, drawStart, drawEnd, lineHeight, texNum, texX);
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
