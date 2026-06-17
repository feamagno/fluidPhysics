#include "render.h"
#include <unistd.h>
#include "simulation.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "point.h"
#include "ball.h"
#include <time.h>

const int WIDTH  = 800;
const int HEIGHT = 800;

const int AMOUNT_OF_BALLS = 1;


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
            nextPosition(ball, delta_s(args->start));
            //printf("thrd: balls[0]: center.x = %lf, center.y = %lf, speed = %lf\n", balls[0].center.x, balls[0].center.y, balls[0].vel);
            //system("sleep 0.01");
        }
}

int main(int argc, char **argv)
{
    struct timeval start;
    gettimeofday(&start, NULL);
    createWindow(WIDTH, HEIGHT);
    Point center = {(double)WIDTH/2, 0};
    Ball ball = {center, delta_s(&start), {0,0}, 20, {0, 0, 255}};
    Ball balls[AMOUNT_OF_BALLS];
    balls[0] = ball;

    struct manage_b arg = {balls, &start};

    pthread_t t1;
    pthread_create(&t1, NULL, manage_balls, &arg);
    
    for (int i = 1; i < 50000; i++ ){
        drawScreen(0,0,0);
        //printf("balls[0]: center = %lf, speed = %lf\n", balls[0].center, balls[0].speed);
        drawBall(balls[0], i);
        //printf("main: balls[0]: center.x = %lf, center.y = %lf, speed = %lf\n", balls[0].center.x, balls[0].center.y, balls[0].vel);
        updateSurface();
    }
    pthread_cancel(t1);
    quit();

    return 0;
}
