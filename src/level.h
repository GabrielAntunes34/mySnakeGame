#ifndef LEVEL_H
    #define LEVEL_H
    #include <stdbool.h>
    #include "snake.h"
    #include "fruit.h"
    #include "pair.h"

    #define MO_CLASSIC 1
    #define MO_ROTTEN 2

    typedef struct level LEVEL;

    LEVEL *levelCreate(int gameMode, int nmrFruits, WINDOW *gameWindow);

    // Getters
    int levelGetScore(LEVEL *level);
    int levelGetSequence(LEVEL *level);
    SNAKE *levelGetSnake(LEVEL *level);
    FRUIT *levelGetFruit(LEVEL *level, bool healthy, int index);
    bool levelEnded(LEVEL *level);
    bool levelWon(LEVEL *level);

    bool levelSetScore(LEVEL *level, int newScore);
    void levelWin(LEVEL *level);

    // Mechanics
    void levelGetUserInput(LEVEL *level);
    bool levelCreateFruit(LEVEL *level, int index, bool healthy);
    bool levelCreateGoldenFruit(LEVEL *level);
    bool levelHandleColisions(LEVEL *level);

    
    bool levelReset(LEVEL *level);
    bool levelDelete(LEVEL **level);

#endif