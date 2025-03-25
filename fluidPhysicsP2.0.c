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

Cell* getCell(int x, int y){
    return &cells[(y * COLUMNS) + x];
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

void moveHorizontal(Cell *cell, int horizontal){
    int direction = horizontal < 0 ? -1 : 1;
    Cell* nextCell;

    //TODO PAAGAR
    // printf("direction: %d, horizontal: %d ", direction, horizontal);
    // printf("horizontal: %d\n", horizontal);
    //TODO APAGAR
    
    int i = 1;
    while (i <= horizontal && (cell->x + i) < ROWS){
        nextCell = getCell(cell->y, cell->x + i);

        //TODO APAGAR
        // printf("%d\n",i);
        // printCell(*nextCell);
        //TODO APAGAR
        
        if (nextCell->type != VOID){
        }

        i++;
    }
}

void moveVertical(Cell *cell, int vertical){
    int direction = vertical < 0 ? -1 : 1;
    Cell* nextCell;

    //TODO PAAGAR
    printf("direction: %d, vertical: %d ", direction, vertical);
    printf("vertical: %d\n", vertical);
    //TODO APAGAR
    
    int i = 1;
    while (i <= vertical && (cell->y + i) < ROWS && (cell->y + i) > 0){
        nextCell = getCell(cell->y + i, cell->x);

        //TODO APAGAR
        printf("%d\n",i);
        printCell(*nextCell);
        //TODO APAGAR
        
        if (nextCell->type != VOID){
        }

        i++;
    }
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
        liquid->vectors[SOUTH] = (downVel + GRAVITY)/liquid->speed;
    }
}

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
            moveHorizontal(cell, horizontal);
            moveVertical(cell, vertical);
            liquid->speed = vertical + horizontal;
        }
    }
}

void step(){
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
        currentFrame = 0;
        nextFrame = 1;
    } else {
        currentFrame = 1;
        nextFrame = 0;
    }
    cells = frames[currentFrame];
}

void duplicateFrames(){
    Cell *cells = frames[currentFrame];
    Cell *next = frames[nextFrame];
    for (int i = 0; i < ROWS*COLUMNS; i++){
        if (cells[i].type == LIQUID){
            next[i] = cells[i];
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
