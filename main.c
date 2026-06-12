#include "render.h"
#include "simulation.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <stdio.h>
#include <stdlib.h>

const int WIDTH  = 800;
const int HEIGHT = 800;

int main(int argc, char **argv)
{
    createWindow(WIDTH, HEIGHT);
    Point center = {(double)WIDTH/2, (double)HEIGHT/2};
    Uint8 color[3] = {0, 0, 255};
    drawBall(center, 200, color);
    system("sleep 10");
    quit();
    return 0;
}

