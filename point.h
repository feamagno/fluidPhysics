#ifndef POINT_H
#define POINT_H

typedef struct {
    double x;
    double y;
} Point;

Point sumPoints(Point p1, Point p2);

Point avgPoints(Point p1, Point p2);

Point mulPoints(Point p1, Point p2);

Point mulPointsConst(Point p1, double a);

#endif
