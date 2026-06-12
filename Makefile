run: render.o main.o simulation.o
	gcc -o run simulation.o render.o main.o `sdl2-config --cflags --libs` -lm

main.o: main.c
	gcc -c main.c 

render.o: render.c
	gcc -c render.c 

simulation.o: simulation.c
	gcc -c simulation.c 

