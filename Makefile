main: main.c
	cc main.c -o main

fluidPhysics: fluidPhysics.c
	cc fluidPhysics.c -o fluidPhysics `sdl2-config --cflags --libs`

