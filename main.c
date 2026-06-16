#include "render.h"
#include "simulation.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "point.h"
#include "ball.h"

const int WIDTH  = 800;
const int HEIGHT = 800;

const int AMOUNT_OF_BALLS = 1;

//Ball balls[AMOUNT_OF_BALLS];

// void* manage_balls(void *arg)
// {
//     while(1){
//         for (int i = 0; i < AMOUNT_OF_BALLS; i++){
//             Ball ball = balls[i];
//             ball.center = nextPosition(ball);
//         }
//     }
// }

int main(int argc, char **argv)
{
    createWindow(WIDTH, HEIGHT);
    Point center = {(double)WIDTH/2, (double)HEIGHT/2};
    Ball ball = {center, {0,0}, 100, {0, 0, 255}};

    //pthread_t t1;
    //pthread_create(&t1, NULL, , NULL);
    //pthread_join(t1, NULL);

    //for (int i = 1; i < 500; i++ ){
       drawScreen(0,0,0);
       //ball.center = nextPosition(ball);
       drawBall(ball, 1);
    //}
    quit();

    printf("regular thread \n");
    return 0;
}
