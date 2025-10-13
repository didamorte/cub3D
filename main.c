/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:41:19 by diogribe          #+#    #+#             */
/*   Updated: 2025/10/13 19:07:05 by diogribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

//Comand to Run:
//cc main.c draw.c -lmlx -lXext -lX11 -lm -o raycaster

#define MOVE_SPEED 0.01
#define ROT_SPEED 0.01

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
// MOVIMENTO - TRANSLAÇÃO
// -----------------------------------------------------
void	handle_player_move(t_game *g)
{
	double	new_x;
	double	new_y;

	if (g->key_w)
	{
		new_x = g->posX + g->dirX * MOVE_SPEED;
		new_y = g->posY + g->dirY * MOVE_SPEED;
		if (g->map[(int)g->posY][(int)new_x] == '0')
			g->posX = new_x;
		if (g->map[(int)new_y][(int)g->posX] == '0')
			g->posY = new_y;
	}
	if (g->key_s)
	{
		new_x = g->posX - g->dirX * MOVE_SPEED;
		new_y = g->posY - g->dirY * MOVE_SPEED;
		if (g->map[(int)g->posY][(int)new_x] == '0')
			g->posX = new_x;
		if (g->map[(int)new_y][(int)g->posX] == '0')
			g->posY = new_y;
	}
}

// -----------------------------------------------------
// MOVIMENTO - ROTAÇÃO
// -----------------------------------------------------
void	handle_player_rotate(t_game *g)
{
	double	old_dir_x;
	double	old_plane_x;
	double	angle;

	if (g->key_a)
		angle = -ROT_SPEED;
	else if (g->key_d)
		angle = ROT_SPEED;
	else
		return ;
	old_dir_x = g->dirX;
	g->dirX = g->dirX * cos(angle) - g->dirY * sin(angle);
	g->dirY = old_dir_x * sin(angle) + g->dirY * cos(angle);
	old_plane_x = g->planeX;
	g->planeX = g->planeX * cos(angle) - g->planeY * sin(angle);
	g->planeY = old_plane_x * sin(angle) + g->planeY * cos(angle);
}

// -----------------------------------------------------
// MOVIMENTO - PRINCIPAL
// -----------------------------------------------------
void	update_movement(t_game *g)
{
	handle_player_move(g);
	handle_player_rotate(g);
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
// FECHAR JANELA (X)
// -----------------------------------------------------
int close_window(t_game *g)
{
	(void)g;
	exit(0);
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
	mlx_hook(g.win, 17, 0, close_window, &g);	 // DestroyNotify (X button)
	mlx_loop_hook(g.mlx, loop_hook, &g);		  // Frame update

	mlx_loop(g.mlx);
}
