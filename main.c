#define _GNU_SOURCE
#include <SDL2/SDL.h>
#include "grid.h"
#include <math.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

/**
 * drawloop - loop to draw terrain
 * @renderer: renderer to use
 * @grid: grid to draw
 * @gwidth: width of grid (in number of nodes)
 * @gheight: height of grid (in number of nodes)
 *
 * Return: 0 on success
 */
int drawloop(SDL_Renderer *renderer, gridnode **grid,
	     int gwidth, int gheight)
{
	SDL_Event event;
	int quit, scale, scalemax, draw = 1;
	double rotate = M_PI / -4, incline = 3 * M_PI / -8;

	scale = SCREEN_HEIGHT / gheight / 1.5;
	scalemax = SCREEN_WIDTH / gwidth / 1.5;
	if (scale > scalemax)
		scalemax = scale;
	scale = scalemax;
	quit = 0;
	while (!quit)
	{
		if (draw == 1)
		{
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0xff, 0xff);
			calculate_grid(grid, scale, incline, rotate,
				       SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, gwidth, gheight);
			draw_grid(grid, renderer);
			SDL_RenderPresent(renderer);
			draw = 0;
		}
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_KEYDOWN &&
			    event.key.keysym.sym == SDLK_ESCAPE)
				quit = 1;
			if (event.type == SDL_KEYDOWN &&
			    event.key.keysym.sym == SDLK_LEFT)
				rotate -= M_PI / 128;
			if (event.type == SDL_KEYDOWN &&
			    event.key.keysym.sym == SDLK_RIGHT)
				rotate += M_PI / 128;
			if (event.type == SDL_KEYDOWN &&
			    event.key.keysym.sym == SDLK_UP)
				incline += M_PI / 256;
			if (event.type == SDL_KEYDOWN &&
			    event.key.keysym.sym == SDLK_DOWN)
				incline -= M_PI / 256;
			if (event.type == SDL_KEYDOWN &&
			    event.key.keysym.sym == SDLK_w)
				if (scale <= scalemax)
					scale += 1;
			if (event.type == SDL_KEYDOWN &&
			    event.key.keysym.sym == SDLK_s)
				if (scale > 5)
					scale -= 1;
			draw = 1;
		}
	}
	return (0);
}

/**
 * main - draw a heightmap grid and modify display of it with UI
 *
 * @ac: number of arguments
 * @av: array of arguments
 *
 * Return: 1 on usage error, -1 on init errors, 0 on success
 */
int main(int ac, char *av[])
{
	SDL_Window *window = NULL;
	SDL_Surface *surface = NULL;
	SDL_Renderer *renderer = NULL;
	int ret = 0;
	unsigned int gwidth = 0, gheight;
	gridnode **grid = NULL;
	FILE *file;

	if (ac < 2)
	{
		dprintf(2, "Usage: terrain filename\n");
		return (1);
	}
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		dprintf(2, "SDL init error: %s\n", SDL_GetError());
		return (-1);
	}
	window = SDL_CreateWindow("Terrain", SDL_WINDOWPOS_UNDEFINED,
				  SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
				  SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		dprintf(2, "Window creation errror: %s", SDL_GetError());
		return (-1);
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		dprintf(2, "Renderer creation error: %s", SDL_GetError());
		return (-1);
	}
	file = fopen(av[1], "r");
	if (file == NULL)
	{
		dprintf(2, "Failed to open grid file %s.\n", av[1]);
		return (-1);
	}
	SDL_UpdateWindowSurface(window);
	surface = SDL_GetWindowSurface(window);
	ret = init_grid(&grid, file, &gwidth, &gheight);
	drawloop(renderer, grid, gwidth, gheight);
	SDL_DestroyRenderer(renderer);
	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return (ret);
}
