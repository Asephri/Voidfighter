/*
Copyright (C) 2023-2025 Asephri.net. All rights reserved.
*/

#include "common.h"
#include "util.h"

int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
	int result = (MAX(x1, x2) < MIN(x1 + w1, x2 + w2)) && (MAX(y1, y2) < MIN(y1 + h1, y2 + h2));

	if (result)
	{
		printf("Collision detected!\n");
	}
	else
	{
		printf("No collision.\n");
	}

	return result;
}

void calcSlope(int x1, int y1, int x2, int y2, float *dx, float *dy)
{
	int steps = MAX(abs(x1 - x2), abs(y1 - y2));

	if (steps == 0)
	{
		*dx = *dy = 0;
		printf("Both points are the same. No slope calculation needed.\n");
		return;
	}

	*dx = (x1 - x2);
	*dx /= steps;

	*dy = (y1 - y2);
	*dy /= steps;

	printf("Slope calculated: dx = %f, dy = %f\n", *dx, *dy);
}