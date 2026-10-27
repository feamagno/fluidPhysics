#include "ball.h"
#include <SDL2/SDL_stdinc.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

const Point null_center = (Point) {-1,-1};
const long double lastAltered = -1;
const double null_radius = -1;
const Ball null_ball = {null_center, null_center, lastAltered, null_center, null_center, null_radius, {0,0,0}};

void inBound(Ball *ball, int width, int height)
{
    ball->last_center = ball->center;
    ball->center.x = MAX(0, MIN(ball->center.x, width  - ball->radius));
    ball->center.y = MAX(0, MIN(ball->center.y, height - ball->radius));
}

Ball createBall(Point center, Point vel, double radius, Uint8 *color)
{
    return (Ball) {center, center, lastAltered, vel, vel, radius, {color[0], color[1], color[2]}};
}

Ball getNullBall()
{
    return null_ball;
}

int isNullBall(Ball ball)
{
    if (ball.radius == null_radius)
        return 1;
    return 0;
}
