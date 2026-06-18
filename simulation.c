#include "simulation.h"
#include "ball.h"
#include "point.h"

const double GRAVITY = 400;

Point calculateGravity(Ball ball, long double time, long double lastAltered)
{
    Point diff = {0, GRAVITY*(time - lastAltered)};
    return diff;
}

void collision(Ball *ball, Point vel, int width, int height)
{
    double x = ball->center.x;
    double y = ball->center.y;
    double radius = ball->radius; 
    if (x >= width - radius || x <= radius)
        ball->vel.x = vel.x * -1;
    else 
        ball->vel.x = vel.x;
    if (y >= height - radius || y <= radius)
        ball->vel.y = vel.y * -1;
    else 
        ball->vel.y = vel.y;
}

Point nextPosition(Ball *ball, long double time, int width, int height)
{
    if (ball->lastAltered != time){
        Point intermediates = calculateGravity(*ball, time, ball->lastAltered);

        Point newVel        = sumPoints(ball->vel, intermediates); 
        Point avgVel        = avgPoints(ball->vel, newVel);
        Point distance      = mulPointsConst(avgVel, time - ball->lastAltered);

        ball->center        = sumPoints(ball->center, distance);
        ball->center        = inBound(*ball, width, height);
        collision(ball, newVel, width, height);
        ball->lastAltered   = time;
    }
    return ball->center;
}


