#CFLAGS   = -g -c -fsanitize=address
#COMPFLAGS = -g -fsanitize=address -o
CFLAGS    = -g -c 
COMPFLAGS = -g -o


run: render.o main.o simulation.o point.o ball.o
	gcc $(COMPFLAGS) run simulation.o render.o main.o ball.o point.o `sdl2-config --cflags --libs` -lm -lpthread

main.o: main.c
	gcc $(CFLAGS) main.c 

render.o: render.c
	gcc $(CFLAGS) render.c 

simulation.o: simulation.c
	gcc $(CFLAGS) simulation.c 

point.o: point.c
	gcc $(CFLAGS) point.c 

ball.o: ball.c
	gcc $(CFLAGS) ball.c 

clean: 
	rm -f *.o 
