#include "grid.h"
#include <stdlib.h>

Ball* ball;
int* removed;


void init(int size)
{
    ball    = malloc(size);
    removed = malloc(size);
}

void insert(Ball ball)
{
    
}

void remove(int x, int y)
{

}

void free_grid()
{
    free(ball);
    free(removed);
    ball    = NULL;
    removed = NULL;
}




