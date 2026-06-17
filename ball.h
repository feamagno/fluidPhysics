#ifndef BALL_H
#define BALL_H

#include "point.h"
#include <SDL2/SDL_stdinc.h>

typedef struct {
    Point center;
    long double lastAltered;
    Point vel;
    double radius;
    Uint8 color[3];
} Ball;

#endif
