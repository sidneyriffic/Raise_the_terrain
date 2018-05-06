#ifndef GRIDH
#define GRIDH

#define BASE_SCALE 50

#include <stdio.h>
#include <SDL2/SDL.h>

/**
 * struct gridnode - base coords for point on grid
 *
 * @x: x coordinate
 * @y: y coordinate
 * @basez: z coordinate
 * @end: whether it is the last node in the array
 */
typedef struct gridnode
{
	int basez, end;
	int x, y;
} gridnode;

int init_grid(gridnode ***grid, FILE *file,
	      unsigned int *width, unsigned int *height);

void calculate_grid(gridnode **grid, int scale, double rotateyz,
		    double rotatexy, int xcent, int ycent, int width, int height);

void draw_grid(gridnode **grid, SDL_Renderer *renderer);

#endif
