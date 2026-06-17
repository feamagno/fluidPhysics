#include "point.h"

Point sumPoints(Point p1, Point p2)
{
    return (Point) {p1.x + p2.x, p1.y + p2.y};
}

Point avgPoints(Point p1, Point p2)
{
    return (Point) {(p1.x + p2.x)/2.0, (p1.y + p2.y)/2.0};
}

Point mulPoints(Point p1, Point p2)
{
    return (Point) {p1.x * p2.x, p1.y * p2.y};
}

Point mulPointsConst(Point p1, double a)
{
    return (Point) {p1.x * a, p1.y * a};
}
