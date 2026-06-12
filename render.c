#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
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

double isInCircle(Point center, double radius, Point point){
    double x = pow(fabs(center.x - point.x), 2);
    double y = pow(fabs(center.y - point.y), 2);
    double root = sqrt(x + y);
    double answer = root <= radius ? root : -1;
    //printf("answer is %lf, root is %lf\n", answer, root);
    return answer;
}

int drawBall(Point center, double radius, Uint8 *color){
    //double s = sin(RADIANS_45);
    double leftMargin, rightMargin;
    double highest = (center.y + radius - 1);
    double lowest  = (center.y - radius - 1);
    double right   = (center.x + radius - 1);
    double left    = (center.x - radius - 1);
    Point point;
    for (double i = lowest; i <= highest; i++){
        double x = left;
        Point point = {x, i};
        double inCircle = isInCircle(center, radius, point);
        while(inCircle < 0 && x < right){
            x++;
            Point point = {x, i};
            inCircle = isInCircle(center, radius, point);
        }
        leftMargin = x < right ? x : -1;
        if (leftMargin != -1){
            while(inCircle > 0 && x < right){ 
                x++;
                Point point = {x, i};
                inCircle = isInCircle(center, radius, point);
            }
            rightMargin = x < right+1 ? x : -1;
            if (rightMargin != -1){
                SDL_Rect rect = {leftMargin, i, rightMargin-leftMargin, 1};
                SDL_FillRect(winSurface, &rect, SDL_MapRGB(winSurface->format, *color, *(color+1), *(color+2)));
            }
        }
        updateSurface();
    }
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
