#include "render.h"
#include <stdio.h>
#include <stdlib.h>
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

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

const int WIDTH  = 800;
const int HEIGHT = 800;

const int AMOUNT_OF_BALLS = 50;

long double delta_sec(struct timeval *start)
{
    struct timeval current;
    gettimeofday(&current, NULL);
    printf("start=%p\n", (void *)start);
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
            if (ball->radius > 0){
                Ball **pinto = &balls;
                nextPosition(i, pinto, AMOUNT_OF_BALLS, delta_sec(args->start), WIDTH, HEIGHT);
            }
        }
}

int main(int argc, char **argv)
{
    struct timeval *start = malloc(sizeof(struct timeval));
    gettimeofday(start, NULL);
    createWindow(WIDTH, HEIGHT);
    //Ball ball  = {center, delta_sec(&start), {0,0}, 40, {0, 0, 255}};
    //Ball ball2 = {center, delta_sec(&start), {0,0}, 0, {0, 0, 255}};
    Ball balls[AMOUNT_OF_BALLS];
    for (int i = 0; i < AMOUNT_OF_BALLS; i++){
        double radius = 10;
        int x = rand();
        int y = rand();
        Point center = {MIN(WIDTH-radius, MAX(x % WIDTH, radius)), MIN(HEIGHT-radius, MAX(y % HEIGHT, radius))};
        balls[i] = createBall(center, (Point) {(x%100)*8,(y%100)*8}, radius, (Uint8[3]) {x%255, y%255, (x+y)%255});
        //balls[i] = (Ball) {center, center, -1, {(x%100)*8,(y%100)*8}, radius, {x%255, y%255, (x+y)%255}};
    }
    struct manage_b arg = {balls, start};

    pthread_t t1;
    pthread_create(&t1, NULL, manage_balls, &arg);
    
    for (int i = 1; i < 25000; i++ ){
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
