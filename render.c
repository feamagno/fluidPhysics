#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdio.h>

#include "render.h"

const char* TITLE  = "Fluid Physics";
const double RADIANS_45 = 0.7853982;

SDL_Surface* winSurface = NULL;
SDL_Window*  window     = NULL;

int createWindow(int width, int height){
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return 1;
    window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, 
                                     SDL_WINDOWPOS_UNDEFINED, 
                                     width, height, SDL_WINDOW_SHOWN);
    if (!window)
        return 1;
    winSurface = SDL_GetWindowSurface(window);
    if (!winSurface)
        return 1;
    return 0;
}

int drawScreen(int r, int g, int b){
    SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, r, g, b));
    return 0;
}

int drawBall(Point center, double radius){
    double s = sin(RADIANS_45);
    printf("%lf\n",s);
    int highest = (int) (center.y + radius - 1);
    int lowest  = (int) (center.y - radius - 1);
    int i = lowest;
    int x = center.x;
    int begin = 
    while (i <= highest){
        

    }
        
    //SDL_FillRect(winSurface, const SDL_Rect *rect, Uint32 color)
    return 0;
}

int madness(int loop){
    for (int i = 0; i < loop; i++){
        switch (i%3){
            case 0: 
                drawScreen(0, 0, 255); break;
            case 1:
                drawScreen(0, 255, 0); break;
            case 2:
                drawScreen(255, 0, 0); break;
        }
        SDL_UpdateWindowSurface(window);
    }
}

int updateSurface(){
    SDL_UpdateWindowSurface(window);
    return 0;
}

int quit(){
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
