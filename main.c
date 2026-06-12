#include "render.h"
#include "simulation.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <stdio.h>

const int WIDTH  = 800;
const int HEIGHT = 800;

int main(int argc, char **argv)
{
    createWindow(WIDTH, HEIGHT);
    Point center = {(double)WIDTH/2, (double)HEIGHT/2};
    Uint8 color[3] = {0, 0, 255};
    drawBall(center, 200, color, 0);
    for (int i = 1; i < 100; i++ ){
        drawScreen(0,0,0);
        center = nextPosition(center, 200);
        printf("after nextPosition %d\n",i);
        drawBall(center, 200, color, i);
        printf("after drawBall %d\n",i);
        updateSurface();
    }
    quit();
    return 0;
}

