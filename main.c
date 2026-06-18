#include "render.h"
#include <stdio.h>
#include <unistd.h>
#include "simulation.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdint.h>
#include "point.h"
#include "ball.h"
#include <time.h>

const int WIDTH  = 800;
const int HEIGHT = 800;

const int AMOUNT_OF_BALLS = 2;


long double delta_s(struct timeval *start)
{
    struct timeval current;
    gettimeofday(&current, NULL);
    long double seconds  = (long double) current.tv_sec - start->tv_sec; // seconds
    long double useconds = ((long double) current.tv_usec/1000000.0 - (long double)start->tv_usec/1000000.0); // microseconds
    seconds += (long double) (useconds); // convert to milliseconds
    return seconds;
}

struct manage_b {
    Ball *ball;  
    struct timeval *start;
};

void* manage_balls(void *arg)
{
    struct manage_b* args = arg;
    Ball* balls = args->ball;
    while(1)
        for (int i = 0; i < AMOUNT_OF_BALLS; i++){
            Ball *ball = balls+i;
            if (ball->radius > 0)
                nextPosition(ball, delta_s(args->start), WIDTH, HEIGHT);
        }
}

int main(int argc, char **argv)
{
    struct timeval start;
    gettimeofday(&start, NULL);
    createWindow(WIDTH, HEIGHT);
    Point center = {(double)WIDTH/2, 140};
    Ball ball  = {center, delta_s(&start), {0,0}, 40, {0, 0, 255}};
    Ball ball2 = {center, delta_s(&start), {0,0}, 0, {0, 0, 255}};
    Ball balls[AMOUNT_OF_BALLS];
    balls[0] = ball;
    balls[1] = ball2;

    struct manage_b arg = {balls, &start};

    pthread_t t1;
    pthread_create(&t1, NULL, manage_balls, &arg);
    
    for (int i = 1; i < 50000; i++ ){
        drawScreen(0,0,0);
        for (int j = 0; j < AMOUNT_OF_BALLS; j++)
            drawBall(balls[j], i);
        updateSurface();
        usleep(200);
    }
    pthread_cancel(t1);
    quit();

    return 0;
}
