#include "render.h"
#include "simulation.h"
#include <stdlib.h>

const int WIDTH  = 800;
const int HEIGHT = 800;

int main(int argc, char **argv)
{
    createWindow(WIDTH, HEIGHT);
    drawBall(5,5);
    system("sleep 5");
    quit();
    return 0;
}

