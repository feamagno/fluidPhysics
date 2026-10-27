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

const int AMOUNT_OF_BALLS = 20;
const int STEPS = 30000;
    
int FINISHED = 1;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

long double delta_sec(struct timeval start, struct timeval current)
{
    long double seconds  = (long double) current.tv_sec - start.tv_sec; // seconds
    long double useconds = ((long double) current.tv_usec - (long double)start.tv_usec)/1000000.0; // microseconds converted to seconds
    seconds += (long double) (useconds); 
    return seconds;
}

struct manage_b {
    Ball *ball;  
    struct timeval *start;
};

void reverse_vel(Ball *balls){
    for (int i = 0; i < AMOUNT_OF_BALLS; i++){
        Ball* ball = balls + 1;
        ball->o_vel = ball->vel;
    }
}

void* manage_balls(void *arg)
{
    struct manage_b* args = arg;
    Ball* balls = args->ball;
    struct timeval start = *(args->start);
    struct timeval *current = malloc(sizeof(struct timeval));
    while(FINISHED == 1){
        gettimeofday(current, NULL);
        long double time_passed = delta_sec(start, *current);
        for (int i = 0; i < AMOUNT_OF_BALLS; i++){
            Ball *ball = balls+i;
            if (ball->radius > 0){
                pthread_mutex_lock(&lock);
                nextPosition(i, balls, AMOUNT_OF_BALLS, time_passed, WIDTH, HEIGHT);
                pthread_mutex_unlock(&lock);
            }
        }
        reverse_vel(balls);
    }
    free(current);
    return NULL;
}

void createBalls(Ball *balls){
    for (int i = 0; i < AMOUNT_OF_BALLS; i++){
        double radius = 20;
        int x = rand();
        int y = rand();
        //int x = WIDTH/2;
        //int y = HEIGHT/5 + i * 5 * radius;
        Point center = {MIN(WIDTH-radius, MAX(x % WIDTH, radius)), MIN(HEIGHT-radius, MAX(y % HEIGHT, radius))};
        balls[i] = createBall(center, (Point) {(x%100)*8,(y%100)*8}, radius, (Uint8[3]) {x%255, y%255, (x+y)%255});
    }
}

int main(int argc, char **argv)
{
    struct timeval *start = malloc(sizeof(struct timeval));
    gettimeofday(start, NULL);
    createWindow(WIDTH, HEIGHT);
    Ball balls[AMOUNT_OF_BALLS];
    createBalls(balls);

    struct manage_b* arg = malloc(sizeof(struct manage_b));
    arg->ball = balls;
    arg->start = start;

    pthread_t t1;
    pthread_create(&t1, NULL, manage_balls, arg);
    
    for (int i = 1; i < STEPS; i++ ){
        drawScreen(0,0,0);
        pthread_mutex_lock(&lock);
        for (int j = 0; j < AMOUNT_OF_BALLS; j++)
            drawBall(balls[j], i);
        pthread_mutex_unlock(&lock);
        updateSurface();
        usleep(200);
    }

    FINISHED = 0;
    pthread_join(t1, NULL);

    quit();
    free(start);
    free(arg);

    return 0;
}
