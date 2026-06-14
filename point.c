#include "point.h"

Point sumPoints(Point p1, Point p2)
{
    return (Point) {p1.x + p2.x, p1.y + p2.y};
}

