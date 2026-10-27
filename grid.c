#include "grid.h"
#include <stdlib.h>

Ball* ball;
int* removed;
int local_size; 

void init(int size)
{
    ball       = malloc(size * sizeof(Ball));
    removed    = malloc(size * sizeof(int));
    local_size = size;
}

void insert(Ball ball)
{
    
}

Ball hasNext();

Ball next();

void rem(int x, int y)
{

}

void free_grid()
{
    free(ball);
    free(removed);
    ball    = NULL;
    removed = NULL;
}

