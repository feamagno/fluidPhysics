run: render.o main.o
	gcc -o run render.o main.o

main.o: main.c
	gcc -c main.c 

render.o: render.c
	gcc -c render.c `sdl2-config --cflags --libs`
