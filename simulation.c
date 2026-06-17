#include "simulation.h"
#include "point.h"
#include <stdio.h>
#include <time.h>

const double GRAVITY = 19.8;

Point calculateGravity(Ball ball, long double time, long double lastAltered)
{
    Point diff = {0, GRAVITY*(time - lastAltered)};
    return diff;
}

Point nextPosition(Ball *ball, long double time)
{
    if (ball->lastAltered != time){
        Point intermediates = calculateGravity(*ball, time, ball->lastAltered);
        Point newVel        = sumPoints(ball->vel, intermediates); 
        Point avgVel        = avgPoints(ball->vel, newVel);
        Point distance      = mulPointsConst(avgVel, time - ball->lastAltered);
        //printf("%Lf\n",time - ball->lastAltered);
        ball->center        = sumPoints(ball->center, distance);
        ball->vel           = newVel;
        ball->lastAltered   = time;
    }
    return ball->center;
}


