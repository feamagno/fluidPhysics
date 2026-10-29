#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "render.h"

const char* TITLE  = "Fluid Physics";
const double RADIANS_45 = 0.7853982;
int this_width;
int this_height;

SDL_Surface* winSurface = NULL;
SDL_Window*  window     = NULL;

int createWindow(int width, int height){
    this_width = width;
    this_height = height;
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

double isInCircle(Ball ball, Point point){
    double x = pow(fabs(ball.center.x - point.x), 2);
    double y = pow(fabs(ball.center.y - point.y), 2);
    double root = sqrt(x + y);
    double answer = root <= ball.radius ? root : -1;
    return answer;
}

void simetricFill(Ball ball, double top, double y_start, int width, int height){
    SDL_Rect rects [4];
    rects[0] = (SDL_Rect) {ball.center.x - width, y_start, 2 * width, height};
    //rects[1] = (SDL_Rect) {ball.center.x - width, y_start, width, height};

    double bottom = top + 2 * ball.radius;
    double top_diff = y_start - top;
    double y_inverted = bottom - top_diff - height;
    rects[1] = (SDL_Rect) {ball.center.x - width, y_inverted, 2 * width, height};
    //rects[3] = (SDL_Rect) {ball.center.x - width, y_inverted, width, height};

    SDL_FillRects(winSurface, rects, 2, SDL_MapRGB(winSurface->format, *ball.color, 
                                                                       *(ball.color+1), 
                                                                       *(ball.color+2)));
}

int checkOnScreen(Ball ball, int j)
{
    if (ball.center.y > this_height + ball.radius || ball.center.y < -ball.radius || ball.center.x > this_width + ball.radius || ball.center.x < -ball.radius){
        return 0;
    }
    return 1;
}

void drawBall(Ball ball, int j){
    if (checkOnScreen(ball, j) < 1) return;
    Point point;
    double lowest  = (ball.center.y);
    double highest = (ball.center.y - ball.radius);
    double y_start = highest;
    int width = 0;
    int height = 1;
    for (double i = highest; i <= lowest; ++i){
        Point point = {ball.center.x+width+1, i};
        double inCircle = isInCircle(ball, point);
        if (inCircle > -1){ //circle has grown
            while (isInCircle(ball, point) > -1){
                width++;
                point = (Point) {ball.center.x+width+1, i};
            }
            simetricFill(ball, highest, y_start, width, height);
            y_start = i+1;
            height = 1;
        }
        else //if we add one, we leave the circle, same size
            height++; 
    }
    simetricFill(ball, highest, y_start, width, height);
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
