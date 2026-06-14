#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL_stdinc.h>
#include "point.h"
#include "ball.h"

int createWindow(int width, int height);

int drawScreen(int r, int g, int b);

int updateSurface();

int madness(int loop);

void drawBall(Ball ball, Uint8 *arr, int i);

int quit();

#endif
