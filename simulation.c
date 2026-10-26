#include "simulation.h"
#include "ball.h"
#include "point.h"
#include <stdio.h>

const double PIXEL_TO_METER = 20; //how many pixels equal a unity of meter 
const double GRAVITY        = 9.8 * PIXEL_TO_METER;
const double ENERGY_COEF    = 0.8;

Point calculateGravity(Ball ball, long double deltaTime)
{
    Point diff = {0, GRAVITY*(deltaTime)};
    return diff;
}

void wall_collision(Ball *ball, Point vel, int width, int height)
{
    double x = ball->center.x;
    double y = ball->center.y;
    double radius = ball->radius; 
    if (x >= width - radius || x <= radius)
        ball->vel.x = vel.x * -ENERGY_COEF;
    else 
        ball->vel.x = vel.x;
    if (y >= height - radius || y <= radius)
        ball->vel.y = vel.y * -ENERGY_COEF;
    else 
        ball->vel.y = vel.y;
}

Point ball_collision(Ball *ball, Ball *balls, int b, int max)
{
    //for (int i = 0; i < )
}

Point nextPosition(int i, Ball *balls, int max, long double time, int width, int height)
{
    Ball *ball = &balls[i]; 

    if (ball->lastAltered == -1)
        ball->lastAltered = time;

    if (ball->lastAltered != time){
        long double deltaTime = time - ball->lastAltered;
        Point oldVel          = ball->vel;
        Point intermediates   = calculateGravity(*ball, deltaTime);
        Point collision       = ball_collision(ball, balls, i, max);

        Point newVel          = sumPoints(ball->vel, intermediates); 
        Point avgVel          = avgPoints(ball->vel, newVel);

        Point distance        = mulPointsConst(avgVel, deltaTime);
        sumToPoint(&ball->center, distance);
        inBound(ball, width, height);

        wall_collision(ball, newVel, width, height);
        ball->lastAltered     = time;
    }
    return ball->center;
}
