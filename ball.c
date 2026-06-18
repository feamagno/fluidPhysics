#include "ball.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

Point inBound(Ball ball, int width, int height)
{
    double x = MAX(0, MIN(ball.center.x, width  - ball.radius));
    double y = MAX(0, MIN(ball.center.y, height - ball.radius));
    return (Point) {x, y};
}

