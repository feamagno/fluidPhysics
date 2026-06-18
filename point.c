#include "point.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

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

void truncatePoint(Point *p, double max, double min)
{
    if (max > -1){
        p->x = MAX(max, p->x);
        p->y = MAX(max, p->y);
    }
                
    if (min > -1){
        p->x = p->x < min ? 0 : p->x;
        p->y = p->y < min ? 0 : p->y;
    }
}
