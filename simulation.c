#include "simulation.h"
#include <stdio.h>

const double GRAVITY = 0;

Point calculateGravity(Point center, double radius){
    double y = center.y;
    Point diff = {0, center.y + GRAVITY};
    return diff;
}

Point nextPosition(Point center, double radius){
    //printf("nextPosition begin \n");
    double final_x = center.x;
    double final_y = center.y;
    Point intermediates = calculateGravity(center, radius);
    final_x += intermediates.x;
    final_y += intermediates.y;
    //printf("nextPosition finishes \n");
    return (Point) {final_x, final_y};
}


