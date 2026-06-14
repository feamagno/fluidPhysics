#include "simulation.h"
#include "point.h"

const double GRAVITY = 2;

Point calculateGravity(Ball ball)
{
    double y = ball.center.y;
    Point diff = {0, GRAVITY};
    return diff;
}

Point nextPosition(Ball ball)
{
    Point intermediates = calculateGravity(ball);
    return sumPoints(ball.center, intermediates); 
}


