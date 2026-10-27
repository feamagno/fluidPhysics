#include "simulation.h"
#include <unistd.h>
#include <sys/time.h>
#include "ball.h"
#include "point.h"
#include "math.h"

const double PIXEL_TO_METER = 40; //how many pixels equal a unity of meter 
const double GRAVITY        = 9.8 * PIXEL_TO_METER;
const double ENERGY_COEF    = 0.7;

Point calculateGravity(Ball ball, long double deltaTime)
{
    Point vector = {0, GRAVITY*(deltaTime)};
    return vector;
}

void wall_collision(Ball *ball, Point vel, int width, int height)
{
    double x      = ball->center.x;
    double y      = ball->center.y;
    double radius = ball->radius; 
    if (x >= width - radius || x <= radius)
        ball->o_vel.x = vel.x * -ENERGY_COEF;
    else 
        ball->o_vel.x = vel.x;
    if (y >= height - radius || y <= radius)
        ball->o_vel.y = vel.y * -ENERGY_COEF;
    else 
        ball->o_vel.y = vel.y;
}

Point ball_collision(Ball ball, Ball *balls, int b, int max)
{
    Point vector = {0,0};
    for (int i = 0; i < max; i++){
        if (i == b) continue;
        Ball o_ball = balls[i];
        double powX = pow(o_ball.center.x - ball.center.x, 2);
        double powY = pow(o_ball.center.y - ball.center.y, 2);
        double dist = sqrt(powX + powY);
        if (dist <= ball.radius + o_ball.radius){
            Point curVec = sumPoints(mulPointsConst(ball.o_vel, -1), mulPointsConst(o_ball.o_vel, -1)); 
            sumToPoint(&vector, curVec);
        }
    }
    return vector;
}

Point nextPosition(int i, Ball *balls, int max, long double time, int width, int height)
{
    Ball *ball = (balls + i); 

    if (ball->lastAltered == -1)
        ball->lastAltered = time;

    if (ball->lastAltered != time){
        long double deltaTime = time - ball->lastAltered;

        Point intermediates   = calculateGravity(*ball, deltaTime);
        Point collision       = ball_collision(*ball, balls, i, max);

        Point newVel          = sumPoints(ball->o_vel, intermediates); 
        sumToPoint(&newVel, collision);
        Point avgVel          = avgPoints(ball->o_vel, newVel);

        Point distance        = mulPointsConst(avgVel, deltaTime);
        sumToPoint(&ball->center, distance);
        inBound(ball, width, height);

        wall_collision(ball, newVel, width, height);
        ball->vel = newVel;
        ball->lastAltered     = time;
    }
    return ball->center;
}
