#ifndef GRID_H
#define GRID_H

#import "ball.h"

typedef struct {
    int size;
    Ball *ball;
} grid ;

void init(int size);

void insert(Ball ball);

void rem(int x, int y);

void free_grid();

#endif
