//cc -o fluidPhysics fluidPhysics.c `sdl2-config --cflags --libs`

#include <stdio.h>
#include <SDL2/SDL.h>

#define WIDTH 900 //1080 900
#define HEIGHT 600 //930 600
#define BLOCK_SIZE 20 //6 20
#define ROWS (HEIGHT/BLOCK_SIZE)
#define COLUMNS (WIDTH/BLOCK_SIZE)
#define SOLID 0
#define LIQUID 1
#define VOID 2
#define FULL_LEVEL 1
#define WHITE 0xffffffff
#define GREEN 0x000fffff
#define BLUE 0x0000ff
#define GRAY  0X52525252
#define DARK_BLUE 0X2394852
#define RED 0xff0000
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define GRAVITY 0.4
#define DEMPENING 0.8
#define FRICTION 0.5
#define NORTH 0
#define SOUTH 1 
#define EAST 2
#define WEST 3
#define TERMINAL_VELOCITY 3

SDL_Surface* surface;
SDL_Window* window;

typedef struct Cell Cell;

typedef struct {
    double speed;
    double vectors[4];
    Cell* cell;
} Liquid;

struct Cell{
    int x;
    int y;
    int type;
    Liquid* liquid;
};

Cell *frames[2];
Cell *cells;

/* DEBUGGING SECTION */
int testedemerda = 1;

void overDrawGravity(Uint32 color, int delay){
    Cell cell;
    for (int i = 0; i < ROWS*COLUMNS; i++){
        cell = cells[i];
        if (cell.type == LIQUID && cell.liquid->vectors[SOUTH] != 0){
            // SDL_Delay(delay);
            SDL_Rect rect = (SDL_Rect) {cell.x*BLOCK_SIZE, cell.y*BLOCK_SIZE, BLOCK_SIZE/8, BLOCK_SIZE/8};
            SDL_FillRect(surface, &rect, color);
            // SDL_UpdateWindowSurface(window);
            // SDL_Delay(delay);
        }
    }

}
void overDrawCell(Cell cell, Uint32 color, int delay){
    SDL_Delay(delay);
    SDL_Rect rect = (SDL_Rect) {cell.x*BLOCK_SIZE, cell.y*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
    SDL_FillRect(surface, &rect, color);
    SDL_UpdateWindowSurface(window);
    SDL_Delay(delay);
}
/* DEBUGGING SECTION */

int drawingType = SOLID;
int blocks[2] = {WHITE, BLUE};
int deleting = 0;
int currentFrame = 0;
int nextFrame = 1;

void initializeWindow(SDL_Window** window, SDL_Surface** surface){
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        exit -1;
    }
    // Create SDL Window
    *window = SDL_CreateWindow(
        "Window Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT, SDL_WINDOW_SHOWN
    );
    if (!*window) {
        printf("Failed to create window: %s\n", SDL_GetError());
        SDL_Quit();
        exit -1;
    }
    *surface = SDL_GetWindowSurface(*window);
}

void passLiquid(Cell *source, Cell *destination){
    Liquid* liquid = source->liquid;
    source->liquid = NULL;
    source->type = VOID;
    destination->liquid = liquid;
    destination->type = LIQUID;
    liquid->cell = destination;
}

void makeCellLiquid(Cell *cell, int speed, double north, double south, double east, double west){
    Liquid* liquid = (Liquid*) malloc(sizeof(Liquid));
    liquid->speed = speed;
    liquid->vectors[SOUTH] = south;
    liquid->vectors[NORTH] = north;
    liquid->vectors[WEST] = west;
    liquid->vectors[EAST] = east;
    liquid->cell = cell;
    cell->type = LIQUID;
    cell->liquid = liquid;
}

void makeCellVoid(Cell* cell){
    cell->type = VOID;
    if (cell->liquid != NULL){
        free(cell->liquid);
        cell->liquid = NULL;
    }
}

void makeCellSolid(Cell* cell){
    cell->type = SOLID;
    if (cell->liquid != NULL){
        free(cell->liquid);
        cell->liquid = NULL;
    }
}

void initializeCells(Cell *cells[]){
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLUMNS; j++){
            Cell cell;
            cell.x = j;
            cell.y = i;
            cell.type = VOID;
            cell.liquid = NULL;
            (*cells)[i*COLUMNS + j] = cell;
        }
}

void intializeFrames(){
    cells = malloc(sizeof(Cell) * ROWS * COLUMNS);
    for(int i = 0; i < 2; i++){
        frames[i] = malloc(COLUMNS * ROWS * (sizeof (Cell)));
        initializeCells(&frames[i]);
    }
    cells = frames[currentFrame];
}

void drawGrid(SDL_Surface* surface, Uint32 color){
    SDL_Rect line = (SDL_Rect) {0, 0, 1, HEIGHT};
    for (int i = 0; i < COLUMNS; i++){
        line.x = i*BLOCK_SIZE;
        SDL_FillRect(surface, &line,color);
    }
    line.x = WIDTH-1;
    SDL_FillRect(surface, &line,color);
    line.x = 0;
    line.h = 1; 
    line.w = WIDTH;
    for (int i = 0; i < ROWS; i++){
        line.y = i*BLOCK_SIZE;
        SDL_FillRect(surface, &line,color);
    }
    line.y = HEIGHT-1;
    SDL_FillRect(surface, &line, color);
}

void printCell(Cell cell){
    printf("x: %d, y: %d, type: %d",cell.x,cell.y,cell.type);
    if (cell.type == LIQUID){
        Liquid* liquid = cell.liquid;
        printf(",speed: %f, north: %f, south: %f, east: %f, west: %f\n",
        liquid->speed,liquid->vectors[0],liquid->vectors[1],liquid->vectors[2],liquid->vectors[3]);
    } else printf("\n");
}

void drawCells(SDL_Surface* surface){
    SDL_Rect draw = (SDL_Rect) {0, 0, BLOCK_SIZE, BLOCK_SIZE};
    for (int i = 0; i < (ROWS)*(COLUMNS); i++){
        Cell *cell = &cells[i];
        if (cell->type == LIQUID){
            draw.x = BLOCK_SIZE * cell->x;
            int waterLevel = BLOCK_SIZE; //TODO THINK ABOUT THIS
            draw.y = (BLOCK_SIZE * cell->y) - (FULL_LEVEL*BLOCK_SIZE - waterLevel);
            draw.h = waterLevel;

            // printf("water\n");
            // printCell(*cell);
            
            SDL_FillRect(surface, &draw, blocks[cell->type]);
        } else if (cell -> type == SOLID){
            draw.x = BLOCK_SIZE * cell->x;
            draw.y = BLOCK_SIZE * cell->y;
            draw.h = BLOCK_SIZE;
            
            // printf("solid\n");
            // printCell(*cell);
            
            SDL_FillRect(surface, &draw, blocks[cell->type]);
        }
    }
}

void handleMouseButton(SDL_Surface* surface, SDL_Event event){
    int x = MIN(event.motion.x, WIDTH-1) / BLOCK_SIZE;
    int y = MIN(event.motion.y, HEIGHT-1) / BLOCK_SIZE;
    x = MAX(x, 0);
    y = MAX(y, 0);
    Cell *cell = &cells[y*COLUMNS + x];
    cell->x = x;
    cell->y = y;
    if (deleting) {
        makeCellVoid(cell);
    }
    else if (drawingType== LIQUID) {
        makeCellLiquid(cell, FULL_LEVEL/2, 0,0,0,0);
    }
    else {
        makeCellSolid(cell);
    }
}

// void createBlock(int x, int y, int type, int speed){
//     Cell *cell = &cells[y*COLUMNS + x];
//     Cell *others = frames[nextFrame];
//     Cell *other = &others[y*COLUMNS + x];
//     cell->x = x;
//     cell->y = y;
//     cell->type = type;
//     cell->speed = speed;
//     other->x = x;
//     other->y = y;
//     other->type = type;
//     other->speed = speed;
// }


void moveHorizontal(Cell *cell, int horizontal){
    int direction = horizontal < 0 ? -1 : 1;
    printf("direction: %d, horizontal: %d", direction, horizontal);
    for (int i = 0; i < horizontal; i++){
        if (cells[(cell->y * COLUMNS) + (cell->x * direction)].type != VOID)
            
    }
}

void moveVertical(Cell *cell, int vertical){

}

/*
 * it is expected that x is valid 
 */
void applyGravity(int x, int y){
    if (y < 0 || y > ROWS){
        printf("Invalid y:%d\n", y);
        exit(1);
    }
    Cell *cell = &cells[y*COLUMNS + x];
    Cell *upper = &cells[(y-1)*COLUMNS + x];
    if (cell->type == LIQUID){
        printCell(*cell);
        Liquid* liquid = cell->liquid;
        if (liquid->speed == 0)
            liquid->vectors[SOUTH] = 1;
        double downVel = liquid->speed * liquid->vectors[SOUTH];
        liquid->speed = liquid->speed + GRAVITY;
        // CALCULATING % OF THE SOUTH SPEED VECTOR
        // TODO SIMPLIFY THIS OPERATION
        liquid->vectors[SOUTH] = (downVel + GRAVITY)/liquid->speed;
    }
}

// void handleSouth(int x, int y, Cell *cell){
//     Cell *nextCells = frames[nextFrame];
//     Cell *nextRead = &cells[(y+1)*COLUMNS + x];
//     Cell *nextWrite = &nextCells[(y+1)*COLUMNS + x];
//     if (nextRead->type == SOLID){
//         cell->vectors[EAST] = cell->vectors[SOUTH]/2;
//         cell->vectors[WEST] = cell->vectors[SOUTH]/2;
//         cell->vectors[SOUTH] = 0;
//     } else if (nextRead->type == LIQUID){
//         if (nextRead->vectors[NORTH] == cell->vectors[SOUTH]){
//             nextWrite->vectors[NORTH] = 0;
//             cell->vectors[SOUTH] = 0;
//         } else {
//             if (nextRead->vectors[NORTH] > cell->vectors[SOUTH]){
//                 nextWrite->vectors[NORTH] -= cell->vectors[SOUTH];
//                 cell->vectors[SOUTH] = 0;
//             } else {
//                 cell->vectors[SOUTH] -= nextWrite->vectors[NORTH];
//                 nextWrite->vectors[NORTH] = 0;
//                 nextWrite->level += cell->level;
//                 makeCellVoid(cell);                             
//             }
//         }
//     } else {
//         nextWrite->level = cell->level;            
//         nextWrite->vectors[SOUTH] = cell->vectors[SOUTH];
//         nextWrite->type = LIQUID; 
//         makeCellVoid(cell);
//     }
// }

// void handleNorth(int x, int y, Cell *cell){
//     Cell *nextCells = frames[nextFrame];
//     Cell *nextRead = &cells[(y-1)*COLUMNS + x];
//     Cell *nextWrite = &nextCells[(y-1)*COLUMNS + x];
//     if (nextRead->type == SOLID){
//         cell->vectors[EAST] = cell->vectors[SOUTH]/2;
//         cell->vectors[WEST] = cell->vectors[SOUTH]/2;
//         cell->vectors[NORTH] = 0;
//     } else if (nextRead->type == LIQUID){
//         if (nextRead->vectors[SOUTH] == cell->vectors[NORTH]){
//             nextWrite->vectors[SOUTH] = 0;
//             cell->vectors[NORTH] = 0;
//         } else {
//             if (nextRead->vectors[SOUTH] > cell->vectors[NORTH]){
//                 nextWrite->vectors[SOUTH] -= cell->vectors[NORTH];
//                 cell->vectors[NORTH] = 0;
//             } else {
//                 cell->vectors[NORTH] -= nextWrite->vectors[SOUTH];
//                 nextWrite->vectors[SOUTH] = 0;
//                 nextWrite->level += cell->level;
//                 makeCellVoid(cell);                             
//             }
//         }
//     } else {
//         nextWrite->level = cell->level;            
//         nextWrite->vectors[NORTH] = cell->vectors[NORTH];
//         nextWrite->type = LIQUID; 
//         makeCellVoid(cell);
//     }
// }

// void handleEast(int x, int y, Cell *cell){
//     Cell *nextCells = frames[nextFrame];
//     Cell *nextRead = &cells[(y)*COLUMNS + (x+1)];
//     Cell *nextWrite = &nextCells[(y)*COLUMNS + (x+1)];
//     if (nextRead->type == SOLID){
//         cell->vectors[NORTH] = cell->vectors[EAST]/2;
//         cell->vectors[SOUTH] = cell->vectors[EAST]/2;
//         cell->vectors[EAST] = 0;
//     } else if (nextRead->type == LIQUID){
//         if (nextRead->vectors[WEST] == cell->vectors[EAST]){
//             nextWrite->vectors[WEST] = 0;
//             cell->vectors[EAST] = 0;
//         } else {
//             if (nextRead->vectors[WEST] > cell->vectors[EAST]){
//                 nextWrite->vectors[WEST] -= cell->vectors[EAST];
//                 cell->vectors[EAST] = 0;
//             } else {
//                 cell->vectors[EAST] -= nextWrite->vectors[WEST];
//                 nextWrite->vectors[WEST] = 0;
//                 nextWrite->level += cell->level;
//                 makeCellVoid(cell);                             
//             }
//         }
//     } else {
//         nextWrite->level = cell->level;            
//         nextWrite->vectors[EAST] = cell->vectors[EAST];
//         nextWrite->type = LIQUID; 
//         makeCellVoid(cell);
//     }
// }

// void handleWest(int x, int y, Cell *cell){
//     Cell *nextCells = frames[nextFrame];
//     Cell *nextRead = &cells[(y)*COLUMNS + (x-1)];
//     Cell *nextWrite = &nextCells[(y)*COLUMNS + (x-1)];
//     if (nextRead->type == SOLID){
//         cell->vectors[NORTH] = cell->vectors[WEST]/2;
//         cell->vectors[SOUTH] = cell->vectors[WEST]/2;
//         cell->vectors[WEST] = 0;
//     } else if (nextRead->type == LIQUID){
//         if (nextRead->vectors[EAST] == cell->vectors[WEST]){
//             nextWrite->vectors[EAST] = 0;
//             cell->vectors[WEST] = 0;
//         } else {
//             if (nextRead->vectors[EAST] > cell->vectors[WEST]){
//                 nextWrite->vectors[EAST] -= cell->vectors[WEST];
//                 cell->vectors[WEST] = 0;
//             } else {
//                 cell->vectors[WEST] -= nextWrite->vectors[EAST];
//                 nextWrite->vectors[EAST] = 0;
//                 nextWrite->level += cell->level;
//                 makeCellVoid(cell);                             
//             }
//         }
//     } else {
//         nextWrite->level = cell->level;            
//         nextWrite->vectors[WEST] = cell->vectors[WEST];
//         nextWrite->type = LIQUID; 
//         makeCellVoid(cell);
//     }
// }

void applyForces(int x, int y){
    if (y < 0 || y > ROWS){
        printf("Invalid y: %d\n", y);
        exit(1);
    }
    Cell *cell = &cells[y*COLUMNS + x];
    if (cell->type == LIQUID){    
        Liquid* liquid = cell->liquid;
        if (liquid->speed != 0){
            double sin = liquid->vectors[NORTH] - liquid->vectors[SOUTH];
            double cos = liquid->vectors[EAST] - liquid->vectors[WEST];
            double vertical = sin*liquid->speed;
            double horizontal = cos*liquid->speed;
            if (vertical < 0)
                vertical *= -1;
            if (horizontal < 0)
                horizontal *= -1;
            moveHorizontal(cell, vertical);
            moveVertical(cell, horizontal);
            liquid->speed = vertical + horizontal;
        }
        // if (!isCellFull(*cell) && cell->type != SOLID && upper->type == LIQUID){
        // if (cell->vectors[SOUTH]){
        //     handleSouth(x, y, cell);
        // }
        // if (cell->vectors[NORTH]){
        //     handleNorth(x, y, cell);
        // }
        // if (cell->vectors[EAST]){
        //     handleEast(x, y, cell);
        // }
        // if (cell->vectors[WEST]){
        //     handleWest(x, y, cell);
        // }
    }
}

void step(){
    // if (testedemerda){
    //     printf("First Cell:\n");
    //     printCell(cells[0]);
    //     testedemerda = 0;
    // }
    for(int i = 0; i < COLUMNS; i++)
        for (int j = 0; j < ROWS; j++)
            applyGravity(i,j);

    for(int i = 0; i < COLUMNS; i++)
        for (int j = 0; j < ROWS-1; j++)
            applyForces(i,j);
}

void cleanScreen(SDL_Surface* surface, SDL_Rect blackScreen, Uint32 color){
    SDL_FillRect(surface,&blackScreen,color);
}

void invertFrames(){
    if (currentFrame) {
        // printf("1\n");
        currentFrame = 0;
        nextFrame = 1;
    } else {
        // printf("2\n");
        currentFrame = 1;
        nextFrame = 0;
    }
    // printf("currentFrame %d\n", currentFrame);
    cells = frames[currentFrame];
}

void duplicateFrames(){
    Cell *cells = frames[currentFrame];
    Cell *next = frames[nextFrame];
    for (int i = 0; i < ROWS*COLUMNS; i++){
        if (cells[i].type == LIQUID){
            next[i] = cells[i];
            // next[i].type = VOID;
            // next[i].liquid = NULL;
        }
        else
            next[i] = cells[i];
    }
}

int main() {
    SDL_Rect blackScreen = (SDL_Rect) {0,0,WIDTH,HEIGHT};
    SDL_Event event;
    initializeWindow(&window, &surface);
    intializeFrames();

    // DEBUGGING
    makeCellLiquid(&cells[0], 0, 0,0,0,0);

    int simulation_is_running = 1;
    while (simulation_is_running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: 
                    simulation_is_running = 0;
                    break;
                case SDL_MOUSEMOTION:
                    if (event.motion.state)
                        handleMouseButton(surface, event);
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_SPACE)
                        drawingType = !drawingType;
                    else if (event.key.keysym.sym == SDLK_DELETE)
                        deleting = !deleting;
                    break;    
            }
        }
        cleanScreen(surface, blackScreen, GRAY);
        drawGrid(surface, DARK_BLUE);
        drawCells(surface);
        duplicateFrames();
        step();
        overDrawGravity(RED, 2);
        invertFrames();
        SDL_UpdateWindowSurface(window);
        SDL_Delay(50);
    }
    free(cells);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}