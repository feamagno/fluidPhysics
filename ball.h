#ifndef BALL_H
#define BALL_H

#include "point.h"
#include <SDL2/SDL_stdinc.h>

typedef struct {
    Point center;
    Point speed;
    double radius;
    Uint8 color[3];
} Ball;

#endif
