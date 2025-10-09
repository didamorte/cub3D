#include <math.h>
#include <stdlib.h>
#include "minilibx-linux/mlx.h"

#define WIDTH 800
#define HEIGHT 600
#define MOVE_SPEED 0.1
#define ROT_SPEED 0.05

typedef struct s_game
{
    void    *mlx;
    void    *win;
    void    *img;
    char    *addr;
    int     bpp;
    int     line_length;
    int     endian;

    char    **map;
    int     mapWidth;
    int     mapHeight;

    double  posX, posY;      // posição do jogador
    double  dirX, dirY;      // direção de visão
    double  planeX, planeY;  // plano de câmera (para FOV)
}   t_game;

static char *g_map[] = {
    "111111111111",
    "100000000001",
    "100000000001",
    "100000000001",
    "100000000001",
    "100011100001",
    "100000000001",
    "100000000001",
    "111111111111",
    NULL
};

// -------------------------------------------------------------------
// PIXEL
// -------------------------------------------------------------------
void my_mlx_pixel_put(t_game *game, int x, int y, int color)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;
    char *dst = game->addr + (y * game->line_length + x * (game->bpp / 8));
    *(unsigned int*)dst = color;
}

// -------------------------------------------------------------------
// DESENHAR COLUNA
// -------------------------------------------------------------------
void draw_vertical_line(t_game *game, int x, int start, int end, int color)
{
    if (start < 0) start = 0;
    if (end >= HEIGHT) end = HEIGHT - 1;
    for (int y = start; y <= end; y++)
        my_mlx_pixel_put(game, x, y, color);
}

// -------------------------------------------------------------------
// RAYCASTING PRINCIPAL
// -------------------------------------------------------------------
void draw_frame(t_game *g)
{
    // limpa imagem (preenche de "céu" e "chão")
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

        int color = side ? 0x00AA00 : 0x00FF00; // paredes mais escuras no eixo Y
        draw_vertical_line(g, x, drawStart, drawEnd, color);
    }
    mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
}

// -------------------------------------------------------------------
// MOVIMENTO E ROTAÇÃO
// -------------------------------------------------------------------
int key_press(int key, t_game *g)
{
    if (key == 65307) // ESC
        exit(0);
    if (key == 119) // W
    {
        if (g->map[(int)(g->posY)][(int)(g->posX + g->dirX * MOVE_SPEED)] == '0')
            g->posX += g->dirX * MOVE_SPEED;
        if (g->map[(int)(g->posY + g->dirY * MOVE_SPEED)][(int)(g->posX)] == '0')
            g->posY += g->dirY * MOVE_SPEED;
    }
    if (key == 115) // S
    {
        if (g->map[(int)(g->posY)][(int)(g->posX - g->dirX * MOVE_SPEED)] == '0')
            g->posX -= g->dirX * MOVE_SPEED;
        if (g->map[(int)(g->posY - g->dirY * MOVE_SPEED)][(int)(g->posX)] == '0')
            g->posY -= g->dirY * MOVE_SPEED;
    }
    if (key == 97) // A
    {
        double oldDirX = g->dirX;
        g->dirX = g->dirX * cos(ROT_SPEED) - g->dirY * sin(ROT_SPEED);
        g->dirY = oldDirX * sin(ROT_SPEED) + g->dirY * cos(ROT_SPEED);
        double oldPlaneX = g->planeX;
        g->planeX = g->planeX * cos(ROT_SPEED) - g->planeY * sin(ROT_SPEED);
        g->planeY = oldPlaneX * sin(ROT_SPEED) + g->planeY * cos(ROT_SPEED);
    }
    if (key == 100) // D
    {
        double oldDirX = g->dirX;
        g->dirX = g->dirX * cos(-ROT_SPEED) - g->dirY * sin(-ROT_SPEED);
        g->dirY = oldDirX * sin(-ROT_SPEED) + g->dirY * cos(-ROT_SPEED);
        double oldPlaneX = g->planeX;
        g->planeX = g->planeX * cos(-ROT_SPEED) - g->planeY * sin(-ROT_SPEED);
        g->planeY = oldPlaneX * sin(-ROT_SPEED) + g->planeY * cos(-ROT_SPEED);
    }
    draw_frame(g);
    return (0);
}


// -------------------------------------------------------------------
// MAIN
// -------------------------------------------------------------------
int main(void)
{
    t_game g;

    g.map = g_map;
    g.mapWidth = 12;
    g.mapHeight = 9;

    g.posX = 3.5;
    g.posY = 3.5;
    g.dirX = -1.0;
    g.dirY = 0.0;
    g.planeX = 0.0;
    g.planeY = 0.66;

    g.mlx = mlx_init();
    g.win = mlx_new_window(g.mlx, WIDTH, HEIGHT, "Raycasting Demo");
    g.img = mlx_new_image(g.mlx, WIDTH, HEIGHT);
    g.addr = mlx_get_data_addr(g.img, &g.bpp, &g.line_length, &g.endian);

    draw_frame(&g);
    mlx_key_hook(g.win, key_press, &g);
    mlx_loop(g.mlx);
}

