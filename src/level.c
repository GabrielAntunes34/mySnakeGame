#include <stdlib.h>
#include <ncurses.h>
#include "ui.h"
#include "level.h"

struct level {
    WINDOW *win;           // Every level happens in a window
    SNAKE *snake;          // Snake controled by the player
    FRUIT **fruits;        // Fruits in the screen that increment the score
    FRUIT **badFruits;     // Unhalthy fruits
    char lastCommand;
    int nmrFruits;         // size of the FRUIT arrays
    int score;             // Player pontuation
    int sequence;          // Sequence of fruits eaten
    bool ended;
    bool won;
};


LEVEL *levelCreate(int gameMode, int nmrFruits, WINDOW *gameWindow) {
    LEVEL *level;
    int maxy;     // Auxiliars to get the maximum width and height of win

    // Allocating memory to the level
    level = (LEVEL *) malloc(sizeof(LEVEL));
    if(level == NULL)
        return NULL;

    // Adjusting the static camps
    level->lastCommand = '0';
    level->score = 0;
    level->sequence = 0;
    level->ended = false;
    level->won = false;
    level->nmrFruits = nmrFruits;
    level->win = gameWindow;

    // Instanciating the snake and the fruit vector
    maxy = getmaxy(gameWindow);

    level->snake = snakeCreate(pairCreate(7, maxy / 2), 3);
    level->fruits = (FRUIT **) malloc(nmrFruits * sizeof(FRUIT *));
    if(level->snake == NULL || level->fruits == NULL)
        return NULL;

    // Generating the position for all fruits
    for(int i = 0; i < nmrFruits; i++)
        levelCreateFruit(level, i, true);

    // Instanciating the badFruits array if needed
    if(gameMode == MO_ROTTEN) {
        level->badFruits = (FRUIT **) malloc(nmrFruits * sizeof(FRUIT *));
        if(level->badFruits == NULL)
            return NULL;

        for(int i = 0; i < nmrFruits; i++)
            levelCreateFruit(level, i, false);
    }
    else
        level->badFruits = NULL;

    firstDrawnSnake(level->win, snakeGetPosition(level->snake), snakeGetSize(level->snake));
    return level;
}

// Getters
int levelGetScore(LEVEL *level) {
    return ((level != NULL) ? level->score : -1);
}

int levelGetSequence(LEVEL *level) {
    return ((level != NULL) ? level->sequence : -1);
}

SNAKE *levelGetSnake(LEVEL *level) {
    return ((level != NULL) ? level->snake : NULL);
}

FRUIT *levelGetFruit(LEVEL *level, bool healthy, int index) {
    if(level == NULL)
        return NULL;
    
    // Verifying if its a fruit or a bad fruit and retrieving it
    if(healthy && index < level->nmrFruits)
        return level->fruits[index];
    else if(!healthy && level->badFruits != NULL && index < level->nmrFruits)
        return level->badFruits[index];
    
    return NULL;
}

bool levelEnded(LEVEL *level) {
    return ((level != NULL) ? level->ended : false);
}

bool levelWon(LEVEL *level) {
    return ((level != NULL) ? level->won : false);
}

bool levelSetScore(LEVEL *level, int newScore) {
    if(level == NULL)
        return false;
    
    level->score = newScore;
    return true;
}

//////////////
// MECHANICS//
//////////////

// Verifies if the level has been won by the size of the snake
// It needs to have the same size as the game window
void levelWin(LEVEL *level) {
    if(level == NULL)
        return;
    int y, x;

    getmaxyx(level->win, y, x);
    if(snakeGetSize(level->snake) == x * y) {
        level->won = true;
        level->ended = true;
    }
    return;
}

void death(LEVEL *level) {
    if(level != NULL) {
        level->ended = true;
        snakeReset(&level->snake, pairCreate(4, getmaxy(level->win) / 2), 3);
        level->sequence = 0;
    }
}

void levelGetUserInput(LEVEL *level) {
    char command;
    PAIR *headPosition = pairCopy(snakeGetPosition(level->snake));

    // Changing the direction of the moviment if anything was pressed and if
    // it's possible to do so. 
    command = getch();
    if((command == 'd' || command == 'D') && level->lastCommand != 'a')
        level->lastCommand = 'd';   // Moving foward
    else if((command == 'a' || command == 'A') && level->lastCommand != 'd')
        level->lastCommand = 'a';   // Moving backward
    else if((command == 'w' || command == 'W') && level->lastCommand != 's')
        level->lastCommand = 'w';   // Moving upward
    else if((command == 's' || command == 'S') && level->lastCommand != 'w')
        level->lastCommand = 's';   // Moving downard

    // Updating snakes position accordingly to the current direction
    switch(level->lastCommand) {
        case 'd':
            pairIncrementX(headPosition);
            break;
        case 'a':
            pairDecrementX(headPosition);
            break;
        case 'w':
            pairDecrementY(headPosition);
            break;
        case 's':
            pairIncrementY(headPosition);
            break;
        default:
            return;
    }

    drawnSnake(level->win, snakeGetTail(level->snake), snakeGetPosition(level->snake), headPosition);
    snakeMove(level->snake, headPosition);
    pairDelete(&headPosition);
    return;
}

// Auxiliar used in all fruits creation, that sets the random position for a new fruit
// whatever the kind
PAIR *levelCreateFruitAux(LEVEL *level) {
    PAIR *randomPos;
    int maxx, maxy;       // Auxiliars to get the position's values
    int value;            // Receives the randon value

    // verifying if it's possible to create a new fruit with the snake's current size
    getmaxyx(level->win, maxy, maxx);
    if((maxy * maxx - snakeGetSize(level->snake)) < 3)
        return NULL;

    // Generating a new position that insn't inside the snake
    // we do (max_ - 2) + 1 to guarantee that it won't appear at any border
    randomPos = pairCreate(-1, -1);
    do {
        pairSetxy(randomPos, (rand() % (maxx - 2)) + 1, (rand() % (maxy - 2)) + 1);
    } while(isInSnake(level->snake, randomPos));

    return randomPos;
}

bool levelCreateFruit(LEVEL *level, int index, bool healthy) {
    if(level == NULL || level->fruits == NULL || index > level->nmrFruits)
        return false;

    // Generating, if it's possible, a random position to the new fruit
    PAIR *randomPos = levelCreateFruitAux(level);
    if(randomPos == NULL)
        return false;

    // Creating the fruit in it's respective array
    if(healthy) {
        level->fruits[index] = fruitCreate(randomPos, healthy);
        drawnFruit(level->win, level->fruits[index]);
    }
    else {
        level->badFruits[index] = fruitCreate(randomPos, healthy);
        drawnFruit(level->win, level->badFruits[index]);
    }
    return true;  
}


bool levelCreateGoldenFruit(LEVEL *level) {
    if(level == NULL)
        return false;

    // auxiliar array to keep all the current unhealthy fruits indexes
    int arr[level->nmrFruits];
    int nmr = 0;   // Index to run through arr
    int choosed;   // Index of the choosed fruit to turn into golden

    // Copying all the disponible indexes
    for(int i = 0; i < level->nmrFruits; i++) {
        if(level->badFruits[i] != NULL && !fruitIsHealthy(level->badFruits[i])) {
            arr[nmr] = i;
            nmr++;
        }
    }

    // Randonly choosing one of the unhealthy fruits to turn into golden
    if(nmr != 0) {
        choosed = rand() % nmr;
        fruitInvertyHealthiness(level->badFruits[arr[choosed]]);
        fruitSetValue(level->badFruits[arr[choosed]], 2 * FRUIT_DF_VALUE);
        drawnFruit(level->win, level->badFruits[arr[choosed]]);
        return true;
    }

    return false;
}

// Auxiliar function to verify if the snake colided with any fruit in a fruit array
// If so, it'll increase it's size and delete the fruit
int fruitColision(LEVEL *level, PAIR *pos, FRUIT **fruits) {
    if(fruits == NULL)
        return -1;
    
    // Verifying through a fruits array if a colision happend
    // If so, the snake will increase in size and the fruit's value will increment the score
    for(int i = 0; i < level->nmrFruits; i++) {
        if(pairCompare(pos, fruitGetPosition(fruits[i]))) {
            snakeIncrease(level->snake);
            level->score += fruitGetValue(fruits[i]);

            fruitDelete(&fruits[i]);
            return i;
        }
    }

    return -1;
}

// Handles all the colisions betwenn the snake and another in-game elements
bool levelHandleColisions(LEVEL *level) {
    if(level == NULL)
        return false;

    int fruitColided;       // If a colision with a fruit happens, it's index is returned here

    // Getting the window's position and limits
    PAIR *snakePos = snakeGetPosition(level->snake);
    int maxx, maxy;
    getmaxyx(level->win, maxy, maxx);

    // Handling colisions with the walls
    // The first col and row of any window is seen as 0 by it's elements
    if(snakeIsTangible(level->snake)) {
        if(pairGetX(snakePos) <= 0 || pairGetX(snakePos) >= maxx - 1
        || pairGetY(snakePos) <= 0 || pairGetY(snakePos) >= maxy - 1) {
            death(level);
            return true;
        }
    }
    // Colision with the snake
    if(snakeInnerColision(level->snake)) {
        death(level);
        return true;
    }
    
    // Colision with a good fruit
    fruitColided = fruitColision(level, snakeGetPosition(level->snake), level->fruits);
    if(fruitColided != -1) {
        level->sequence++;
        drawnScore(level);
        levelCreateFruit(level, fruitColided, true);
        return true;
    }
    // Colision with a bad or golden fruit
    fruitColided = fruitColision(level, snakeGetPosition(level->snake), level->badFruits);
    if(fruitColided != -1) {
        level->sequence = 0;
        drawnScore(level);
        levelCreateFruit(level, fruitColided, false);
        return true;
    }

    return false;
}


bool levelReset(LEVEL *level);

bool levelDelete(LEVEL **level) {
    if(*level != NULL) {
        // Desallocating the snake
        snakeDelete(&(*level)->snake);

        // Desalocating the fruits'arrays
        for(int i = 0; i < (*level)->nmrFruits; i++) {
            fruitDelete(&(*level)->fruits[i]);
            if((*level)->badFruits != NULL)
                fruitDelete(&(*level)->badFruits[i]);
        }
        free((*level)->fruits);
        if((*level)->badFruits != NULL)
            free((*level)->badFruits);

        free(*level);
        level = NULL;
        return true;
    }
    return false;
}