#define _GNU_SOURCE
#include "grid.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>

/**
 * init_grid - initialized a grid matrix. Note that it is
 * not assigned to a single memory block.
 *
 * @gridref: variable to set in caller once we're done
 * @file: file to read from
 * @width: variable to set with grid width
 * @height: variable to set with grid height
 *
 * Return: 0 on success, -1 on failure
 */
int init_grid(gridnode ***gridref, FILE *file,
	      unsigned int *width, unsigned int *height)
{
	char *buffer, *tok;
	unsigned int xalloc = 1, yalloc = 1, x = 0, y = 0;
	size_t n = 0;
	gridnode **grid = NULL;

	while (getline(&buffer, &n, file) != -1)
	{
		if (yalloc < y + 2)
		{
			yalloc *= 2;
			grid = realloc(grid, sizeof(gridnode *) * yalloc);
		}
		tok = strtok(buffer, " ");
		grid[y] = malloc(sizeof(gridnode));
		grid[y][0].end = 1;
		while (tok != NULL && *tok != '\n')
		{
			if (xalloc < x + 2)
			{
				xalloc *= 2;
				grid[y] = realloc(grid[y], sizeof(gridnode) * xalloc);
			}
			grid[y][x].end = 0;
			grid[y][x].basez = atoi(tok);
			x++;
			tok = strtok(NULL, " ");
		}
		grid[y][x].end = 1;
		y++;
		if (x > *width)
			*width = x;
		x = 0;
		xalloc = 1;
		free(buffer);
		buffer = NULL;
		n = 0;
	}
	*height = y + 1;
	++*width;
	grid[y] = NULL;
	*gridref = grid;
	return (0);
}

/**
 * calculate_grid - calculates the renderable grid coordinates
 * from the base coordinates
 *
 * @grid: grid to calculate
 * @scale: scale for grid
 * @rotateyz: rotation around yz axis
 * @rotatexy: rotation around xy axis
 * @xcent: x center
 * @ycent: y center
 * @width: width of grid
 * @height: height of grid
 *
 *Return: void
 */
void calculate_grid(gridnode **grid, int scale, double rotateyz,
		    double rotatexy, int xcent, int ycent, int width, int height)
{
	int x = 0, y = 0;
	double offx, offy;

	while (grid[y] != NULL)
	{
		while (!grid[y][x].end)
		{
			offx = scale * (x + 1 - width / 2.0);
			offy = scale * (y + 1 - height / 2.0);
			grid[y][x].x = (offx * cos(rotatexy) - offy *  sin(rotatexy)) + xcent;
			offy = offy * cos(rotatexy) + offx * sin(rotatexy);
			grid[y][x].y = ((offy * cos(rotateyz) +
					 grid[y][x].basez * scale / BASE_SCALE * sin(rotateyz))	+ ycent);
			x++;
		}
		y++;
		x = 0;
	}
}

/**
 * draw_grid - draw a grid
 *
 * @grid: grid to draw
 * @renderer: renderer to use
 *
 * Return: void
 */
void draw_grid(gridnode **grid, SDL_Renderer *renderer)
{
	unsigned int x = 0, y = 0, nextlen;

	while (grid[y] != NULL)
	{
		nextlen = 0;
		if (grid[y + 1] != NULL)
			while (grid[y + 1][x].end != 1)
			{
				x++;
				nextlen++;
			}
		x = 0;
		while (!grid[y][x].end)
		{
			if (!grid[y][x + 1].end)
				SDL_RenderDrawLine(renderer, grid[y][x].x,
						   grid[y][x].y, grid[y][x + 1].x,
						   grid[y][x + 1].y);
			if (x < nextlen)
				SDL_RenderDrawLine(renderer, grid[y][x].x, grid[y][x].y,
						   grid[y + 1][x].x, grid[y + 1][x].y);
			x++;
		}
		y++;
		x = 0;
	}
}
