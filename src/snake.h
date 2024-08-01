#ifndef SNAKE_H
    #define SNAKE_H
    #include <stdbool.h>
    #include "pair.h"

    // Constants to drawn the snake with ncurses
    #define SNAKE_HEAD '#'
    #define SNAKE_BODY '*'
    #define SNAKE_TAIL '*'

    // SNAKE works as a linked list where the head represents it's position and directions
    // While all the other nodes may be also tangible and follow it.
    typedef struct snake SNAKE;


    SNAKE *snakeCreate(PAIR *position, int size);

    // Getters
    PAIR *snakeGetPosition(SNAKE *snake);
    PAIR *snakeGetTail(SNAKE *snake);
    int snakeGetSize(SNAKE *snake);
    bool snakeIsTangible(SNAKE *snake);

    // Mechanics of the snake
    bool snakeInvertTangibility(SNAKE *snake);
    bool snakeIncrease(SNAKE *snake);
    bool snakeMove(SNAKE *snake, PAIR *NewHeadPosition);
    bool snakeInnerColision(SNAKE *snake);

    void snakeReset(SNAKE **snake, PAIR *stdPos, int stdSize);
    bool snakeDelete(SNAKE **snake);

#endif