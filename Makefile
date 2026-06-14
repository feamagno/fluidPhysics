run: render.o main.o simulation.o point.o
	gcc -o run simulation.o render.o main.o point.o `sdl2-config --cflags --libs` -lm

main.o: main.c
	gcc -c main.c 

render.o: render.c
	gcc -c render.c 

simulation.o: simulation.c
	gcc -c simulation.c 

point.o: point.c
	gcc -c point.c 

clean: 
	rm -f *.o 
