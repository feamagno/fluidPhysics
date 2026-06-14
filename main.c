#include "render.h"
#include "simulation.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <stdlib.h>
#include "point.h"

const int WIDTH  = 800;
const int HEIGHT = 800;

int main(int argc, char **argv)
{
    createWindow(WIDTH, HEIGHT);
    Point center = {(double)WIDTH/2, (double)HEIGHT/2};
    Uint8 color[3] = {0, 0, 255};

    for (int i = 1; i < 1000; i++ ){
       drawScreen(0,0,0);
       center = nextPosition(center, 100);
       drawBall(center, 100, color, i);
       updateSurface();
        system("sleep 0.1");
    }
    quit();
    return 0;
}
