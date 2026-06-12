#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL_stdinc.h>

typedef struct {
    double x;
    double y;
} Point;

int createWindow(int width, int height);

int drawScreen(int r, int g, int b);

int updateSurface();

int madness(int loop);

int drawBall(Point center, double radius, Uint8 *arr);

int quit();

#endif
