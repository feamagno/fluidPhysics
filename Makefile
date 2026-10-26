run: render.o main.o simulation.o point.o ball.o
	gcc -g -o run simulation.o render.o main.o ball.o point.o `sdl2-config --cflags --libs` -lm -lpthread

main.o: main.c
	gcc -g -c main.c 

render.o: render.c
	gcc -g -c render.c 

simulation.o: simulation.c
	gcc -g -c simulation.c 

point.o: point.c
	gcc -g -c point.c 

ball.o: ball.c
	gcc -g -c ball.c 

clean: 
	rm -f *.o 
