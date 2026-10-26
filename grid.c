#include "grid.h"
#include <stdlib.h>

Ball* ball;
int* removed;


void init(int size)
{
    ball    = malloc(size * sizeof(Ball));
    removed = malloc(size * sizeof(int));
}

void insert(Ball ball)
{
    
}

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




