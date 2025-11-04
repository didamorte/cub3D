/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 20:45:00 by diogribe          #+#    #+#             */
/*   Updated: 2025/11/04 21:54:35 by diogribe         ###   ########.fr       */
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
void	clear_screen(t_game *g)
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
// OBTER COR DA TEXTURA
// -----------------------------------------------------
int	get_texture_color(t_game *g, t_wall *w, int tex_y)
{
	t_texture	*tex;
	int			bpp_div;

	tex = &g->textures[w->tex_num];
	bpp_div = tex->bpp / 8;
	return (*(unsigned int *)(tex->addr
		+ (tex_y * tex->line_length + w->tex_x * bpp_div)));
}

// -----------------------------------------------------
// INICIALIZAR TEXTURA DRAW
// -----------------------------------------------------
void	init_tex_draw(t_tex_draw *td, t_wall *w, int tex_height)
{
	td->step = 1.0 * tex_height / w->line_height;
	td->tex_pos = (w->draw_start - HEIGHT / 2 + w->line_height / 2) * td->step;
	if (w->draw_start < 0)
	{
		td->tex_pos += td->step * (-w->draw_start);
		w->draw_start = 0;
	}
	if (w->draw_end >= HEIGHT)
		w->draw_end = HEIGHT - 1;
}
