#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> // para strdup()
#include "mlx.h"

#define WIDTH 800
#define HEIGHT 600
#define MOVE_SPEED 0.01
#define ROT_SPEED 0.01

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

static char *g_map[] = {
	"111111",
	"100001",
	"10N101",
	"100001",
	"111111",
	NULL
};

// -----------------------------------------------------
// DUPLICA MAPA
// -----------------------------------------------------
char **dup_map(char **src)
{
    int i, len = 0;
    while (src[len])
        len++;
    char **dst = malloc((len + 1) * sizeof(char *));
    for (i = 0; i < len; i++)
        dst[i] = strdup(src[i]);
    dst[i] = NULL;
    return dst;
}

// -----------------------------------------------------
// CARREGAR TEXTURAS XPM
// -----------------------------------------------------
void	load_texture(t_game *g, int index, char *path)
{
	g->textures[index].img = mlx_xpm_file_to_image(g->mlx, path,
		&g->textures[index].width, &g->textures[index].height);
	if (!g->textures[index].img)
	{
		fprintf(stderr, "Erro: não foi possível carregar %s\n", path);
		exit(1);
	}
	g->textures[index].addr = mlx_get_data_addr(g->textures[index].img,
		&g->textures[index].bpp, &g->textures[index].line_length,
		&g->textures[index].endian);
}

void	create_textures(t_game *g)
{
	load_texture(g, 0, "textures/east.xpm");
	load_texture(g, 1, "textures/west.xpm");
	load_texture(g, 2, "textures/south.xpm");
	load_texture(g, 3, "textures/north.xpm");
}

// -----------------------------------------------------
// PIXEL
// -----------------------------------------------------
void my_mlx_pixel_put(t_game *game, int x, int y, int color)
{
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return;
	char *dst = game->addr + (y * game->line_length + x * (game->bpp / 8));
	*(unsigned int*)dst = color;
}

// -----------------------------------------------------
// DESENHAR COLUNA
// -----------------------------------------------------
void draw_vertical_line(t_game *game, int x, int start, int end, int color)
{
	if (start < 0) start = 0;
	if (end >= HEIGHT) end = HEIGHT - 1;
	for (int y = start; y <= end; y++)
		my_mlx_pixel_put(game, x, y, color);
}

// -----------------------------------------------------
// RAYCASTING PRINCIPAL
// -----------------------------------------------------
void draw_frame(t_game *g)
{
	// limpa imagem (céu + chão)
	for (int y = 0; y < HEIGHT; y++)
	{
		int color = (y < HEIGHT / 2) ? 0x87CEEB : 0x444444;
		for (int x = 0; x < WIDTH; x++)
			my_mlx_pixel_put(g, x, y, color);
	}

	for (int x = 0; x < WIDTH; x++)
	{
		double cameraX = 2 * x / (double)WIDTH - 1;
		double rayDirX = g->dirX + g->planeX * cameraX;
		double rayDirY = g->dirY + g->planeY * cameraX;

		int mapX = (int)g->posX;
		int mapY = (int)g->posY;

		double deltaDistX = fabs(1 / rayDirX);
		double deltaDistY = fabs(1 / rayDirY);

		double sideDistX;
		double sideDistY;
		int stepX, stepY;

		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (g->posX - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0 - g->posX) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (g->posY - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0 - g->posY) * deltaDistY;
		}

		int hit = 0;
		int side;
		while (hit == 0)
		{
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			if (g->map[mapY][mapX] == '1')
				hit = 1;
		}

		double perpWallDist = (side == 0)
			? (mapX - g->posX + (1 - stepX) / 2) / rayDirX
			: (mapY - g->posY + (1 - stepY) / 2) / rayDirY;

		int lineHeight = (int)(HEIGHT / perpWallDist);
		int drawStart = -lineHeight / 2 + HEIGHT / 2;
		int drawEnd = lineHeight / 2 + HEIGHT / 2;

		int texNum = 0;
		if (side == 0 && rayDirX > 0)
			texNum = 0;
		else if (side == 0 && rayDirX < 0)
			texNum = 1;
		else if (side == 1 && rayDirY > 0)
			texNum = 2;
		else if (side == 1 && rayDirY < 0)
			texNum = 3;

		double wallX;
		if (side == 0)
			wallX = g->posY + perpWallDist * rayDirY;
		else
			wallX = g->posX + perpWallDist * rayDirX;
		wallX -= floor(wallX);

		int texX = (int)(wallX * (double)g->textures[texNum].width);
		if ((side == 0 && rayDirX > 0) || (side == 1 && rayDirY < 0))
			texX = g->textures[texNum].width - texX - 1;
		if (texX < 0)
			texX = 0;
		if (texX >= g->textures[texNum].width)
			texX = g->textures[texNum].width - 1;

		double step = 1.0 * g->textures[texNum].height / lineHeight;
		double texPos = (drawStart - HEIGHT / 2 + lineHeight / 2) * step;

		if (drawStart < 0)
		{
			texPos += step * (-drawStart);
			drawStart = 0;
		}
		if (drawEnd >= HEIGHT)
			drawEnd = HEIGHT - 1;

		t_texture *tex = &g->textures[texNum];
		int tex_bpp_div = tex->bpp / 8;

		for (int y = drawStart; y <= drawEnd; y++)
		{
			int texY = (int)texPos;
			if (texY >= tex->height)
				texY = tex->height - 1;
			else if (texY < 0)
				texY = 0;
			int color = *(unsigned int *)(tex->addr
				+ (texY * tex->line_length + texX * tex_bpp_div));
			my_mlx_pixel_put(g, x, y, color);
			texPos += step;
		}
	}

	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
}

// -----------------------------------------------------
// POSIÇÃO DO PLAYER
// -----------------------------------------------------
void find_player_start(t_game *g)
{
	for (int y = 0; g->map[y]; y++)
	{
		for (int x = 0; g->map[y][x]; x++)
		{
			char c = g->map[y][x];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				g->posX = x + 0.5;
				g->posY = y + 0.5;
				if (c == 'N')
				{
					g->dirX = 0; g->dirY = -1;
					g->planeX = 0.66; g->planeY = 0;
				}
				if (c == 'S')
				{
					g->dirX = 0; g->dirY = 1;
					g->planeX = -0.66; g->planeY = 0;
				}
				if (c == 'E')
				{
					g->dirX = 1; g->dirY = 0;
					g->planeX = 0; g->planeY = 0.66;
				}
				if (c == 'W')
				{
					g->dirX = -1; g->dirY = 0;
					g->planeX = 0; g->planeY = -0.66;
				}
				g->map[y][x] = '0'; // substitui o N/S/E/W por chão
				return;
			}
		}
	}
	fprintf(stderr, "Erro: posição inicial do jogador não encontrada!\n");
	exit(1);
}

// -----------------------------------------------------
// MOVIMENTO
// -----------------------------------------------------
void update_movement(t_game *g)
{
	if (g->key_w)
	{
		if (g->map[(int)(g->posY)][(int)(g->posX + g->dirX * MOVE_SPEED)] == '0')
			g->posX += g->dirX * MOVE_SPEED;
		if (g->map[(int)(g->posY + g->dirY * MOVE_SPEED)][(int)(g->posX)] == '0')
			g->posY += g->dirY * MOVE_SPEED;
	}
	if (g->key_s)
	{
		if (g->map[(int)(g->posY)][(int)(g->posX - g->dirX * MOVE_SPEED)] == '0')
			g->posX -= g->dirX * MOVE_SPEED;
		if (g->map[(int)(g->posY - g->dirY * MOVE_SPEED)][(int)(g->posX)] == '0')
			g->posY -= g->dirY * MOVE_SPEED;
	}
	if (g->key_a)
	{
		double oldDirX = g->dirX;
		g->dirX = g->dirX * cos(-ROT_SPEED) - g->dirY * sin(-ROT_SPEED);
		g->dirY = oldDirX * sin(-ROT_SPEED) + g->dirY * cos(-ROT_SPEED);
		double oldPlaneX = g->planeX;
		g->planeX = g->planeX * cos(-ROT_SPEED) - g->planeY * sin(-ROT_SPEED);
		g->planeY = oldPlaneX * sin(-ROT_SPEED) + g->planeY * cos(-ROT_SPEED);
	}
	if (g->key_d)
	{
		double oldDirX = g->dirX;
		g->dirX = g->dirX * cos(ROT_SPEED) - g->dirY * sin(ROT_SPEED);
		g->dirY = oldDirX * sin(ROT_SPEED) + g->dirY * cos(ROT_SPEED);
		double oldPlaneX = g->planeX;
		g->planeX = g->planeX * cos(ROT_SPEED) - g->planeY * sin(ROT_SPEED);
		g->planeY = oldPlaneX * sin(ROT_SPEED) + g->planeY * cos(ROT_SPEED);
	}
}

// -----------------------------------------------------
// INPUT HANDLERS
// -----------------------------------------------------
int key_press(int key, t_game *g)
{
	if (key == 65307) exit(0);
	if (key == 119) g->key_w = 1; // W
	if (key == 115) g->key_s = 1; // S
	if (key == 97)  g->key_a = 1; // A
	if (key == 100) g->key_d = 1; // D
	return (0);
}

int key_release(int key, t_game *g)
{
	if (key == 119) g->key_w = 0;
	if (key == 115) g->key_s = 0;
	if (key == 97)  g->key_a = 0;
	if (key == 100) g->key_d = 0;
	return (0);
}

// -----------------------------------------------------
// LOOP PRINCIPAL
// -----------------------------------------------------
int loop_hook(t_game *g)
{
	update_movement(g);
	draw_frame(g);
	return (0);
}

// -----------------------------------------------------
// MAIN
// -----------------------------------------------------
int main(void)
{
	t_game g;

	g.map = dup_map(g_map);
	find_player_start(&g);
	g.key_w = g.key_a = g.key_s = g.key_d = 0;

	g.mlx = mlx_init();
	g.win = mlx_new_window(g.mlx, WIDTH, HEIGHT, "Raycasting - Linux");
	g.img = mlx_new_image(g.mlx, WIDTH, HEIGHT);
	g.addr = mlx_get_data_addr(g.img, &g.bpp, &g.line_length, &g.endian);

	create_textures(&g);

	mlx_hook(g.win, 2, 1L<<0, key_press, &g);	 // KeyPress
	mlx_hook(g.win, 3, 1L<<1, key_release, &g);   // KeyRelease
	mlx_loop_hook(g.mlx, loop_hook, &g);		  // Frame update

	mlx_loop(g.mlx);
}
