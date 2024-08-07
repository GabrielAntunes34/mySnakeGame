#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "level.h"
#include "ui.h"

// Manages the main loop of gameplay for all gameModes
int gameLoop(LEVEL *level) {
    // the loop ends when the player dies or complete the level
    while(!levelEnded(level)) {
        levelGetUserInput(level);
        levelHandleColisions(level);
        levelWin(level);
        napms(100);
    }

    // Verifying if the level ended with a death or completed
    if(levelWon(level))
        return 5;
    else
        return 4;
}

int gameLoopRotten(LEVEL *level) {
    // the loop ends when the player dies or complete the level
    while(!levelEnded(level)) {

        levelGetUserInput(level);
        levelHandleColisions(level);

        if(levelGetSequence(level) % 5 == 0 && levelGetSequence(level) != 0)
            levelCreateGoldenFruit(level);

        levelWin(level);
        napms(100);
    }

    // Verifying if the level ended with a death or completed
    if(levelWon(level))
        return 5;
    else
        return 4;
}

int main() {
    LEVEL *level;
    WINDOW *gameWindow;     // Keeps the ncurses' window used in a game loop
    int globalScore = 0;    // Tracks the score of the player in case he wins
    PAIR *scrDimensions;    // Keeps the maximum width and heigth of the terminal opened
    char opCode;            // Used to handle the user input for menu's navigation
    int menuOption = 0;     // Used to handle the option of the menu choosed
    bool running = true;    // Used to verify if the game has ended or not

    // Setting a seed to randonly generate fruit positions
    srand(time(NULL));

    // Initializing the game in the menu screen
    scrDimensions = initialize();
    
    while(running) {
        // Handling the moviment in the menu
        opCode = getch();
        if(opCode == 'w' || opCode == 'W')
            moveCursorUp(scrDimensions);
        else if(opCode == 's' || opCode == 'S')
            moveCursorDown(scrDimensions);
        else if(opCode == 10)
            menuOption = enterOption(scrDimensions);

        // Handling the option of the menu entered
        switch(menuOption) {
            case 1:     // Game mode 1 entered
                gameWindow = renderGameScreen(scrDimensions);
                level = levelCreate(MO_CLASSIC, 1, gameWindow);
                drawnScore(level);
                menuOption = gameLoop(level);
                break;

            case 2:    // Game mode 2 entered
                gameWindow = renderGameScreen(scrDimensions);
                level = levelCreate(MO_ROTTEN, 1, gameWindow);
                drawnScore(level);
                menuOption = gameLoopRotten(level);
                break;

            case 3:    // Exit entered
                running = false;
                break;

            case 4:    // If player died in a loop
                delwin(gameWindow);
                clear();
                drawnMenuScreen(scrDimensions);
                menuOption = 0;
                break;

            case 5:    // if the player wins a level
                //levelReset();
                //opCode = gameLoop(level);
                break;
        }
    }

    pairDelete(&scrDimensions);
    levelDelete(&level);
    endwin();
    return 0;
}

/*
int main() {
    PAIR *headPosition;
    SNAKE *ss;
    FRUIT *apple;

    // Instanciating our snake
    headPosition = pairCreate(10, 5);
    if(headPosition == NULL)
        return 1;
    ss = snakeCreate(headPosition, 14);
    if(ss == NULL)
        return 1;


    // SCREEEEEEEEEEEEEN
    initscr();
    curs_set(0);
    noecho();

    // Would you lika an apple my Dear???
    apple = fruitCreate(pairCreate(12, 12), true);
    if(!fruitIsHealthy(apple) && !fruitIsTangible(apple)) {
        move(pairGetY(fruitGetPosition(apple)), pairGetX(fruitGetPosition(apple)));
        addch('A');
    }

    // Drawing our little snake
    firstDrawnSnake(headPosition, snakeGetSize(ss));
    // Let's move this snake a bit
    getch();
    for(int j = 0; j < 2; j++) {
        // Moving a bit foward
        for(int i = 0; i < 3; i++) {
            move(0, i);
            addch('d');

            pairIncrementX(headPosition);
            drawnSnake(snakeGetTail(ss), snakeGetPosition(ss), headPosition);
            snakeMove(ss, headPosition);

            napms(500);
        }

        // going five below :)
        for(int i = 0; i < 2; i++) {
            move(i + 1, 2);
            addch('s');

            pairIncrementY(headPosition);
            drawnSnake(snakeGetTail(ss), snakeGetPosition(ss), headPosition);
            snakeMove(ss, headPosition);

            napms(500);
        }

        // A little do the left
        for(int i = 0; i < 2; i++) {
            move(2, 1 - i);
            addch('a');

            pairDecrementX(headPosition);
            drawnSnake(snakeGetTail(ss), snakeGetPosition(ss), headPosition);
            snakeMove(ss, headPosition);

            napms(500);
        }

        // NOW HIT THIS SON OF A $%#%!!!!
        for(int i = 0; i < 2; i++) {
            move(1 - i, 0);
            addch('a');

            pairDecrementY(headPosition);
            drawnSnake(snakeGetTail(ss), snakeGetPosition(ss), headPosition);
            snakeMove(ss, headPosition);

            if(snakeInnerColision(ss)) {
                moveaddch(15,15, 'F');
                getch();
                endwin();
                return 0;
            }

            napms(500);
        }
    }

    getch();
    endwin();

    //printf("Alguem está a segfaultar\n");
    //printf("fdsafklsajfsa;fjsda\n\n");

    pairDelete(&headPosition);
    snakeDelete(&ss);
    fruitDelete(&apple);
    return 0;
}*/