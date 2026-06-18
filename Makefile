run: render.o main.o simulation.o point.o ball.o
	gcc -o run simulation.o render.o main.o ball.o point.o `sdl2-config --cflags --libs` -lm -lpthread

main.o: main.c
	gcc -c main.c 

render.o: render.c
	gcc -c render.c 

simulation.o: simulation.c
	gcc -c simulation.c 

point.o: point.c
	gcc -c point.c 

ball.o: ball.c
	gcc -c ball.c 

clean: 
	rm -f *.o 
