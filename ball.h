#ifndef BALL_H
#define BALL_H

#include "point.h"
#include <SDL2/SDL_stdinc.h>

typedef struct {
    Point center;
    Point last_center;
    long double lastAltered;
    Point vel;
    Point o_vel;
    double radius;
    Uint8 color[3];
} Ball;

void inBound(Ball *ball, int width, int height);

Ball createBall(Point center, Point vel, double radius, Uint8 *color);

Ball getNullBall();

int isNullBall(Ball ball);

#endif
