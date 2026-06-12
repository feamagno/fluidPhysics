#ifndef RENDER_H
#define RENDER_H

typedef struct {
    double x;
    double y;
} Point;

int createWindow(int width, int height);

int drawScreen(int r, int g, int b);

int updateSurface();

int madness(int loop);

int drawBall(Point center, double radius);

int quit();

#endif
