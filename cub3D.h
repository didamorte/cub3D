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
void	draw_vertical_line(t_game *game, int x, int start, int end, int color);
void	draw_frame(t_game *g);

#endif
