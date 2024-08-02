#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "level.h"

#define txtButton1 "   Classic   "
#define txtButton2 "Rotten fruits"
#define txtButton3 "  Quit game  "
#define txtScore "SCORE: 00000000000"

#define gameWidth 50
#define gameHeight 50

// MUDAR????

// MANTER: FUNCIONA PARA REDESENHÁ-LA SEMPRE


// UI function to drwan the menu screen of the game
void drawnMenuScreen(PAIR *scrDimensions) {
    // Title length: 10
    int titlePosX  = (pairGetX(scrDimensions) - 10)/ 2;
    int buttonPosX = (pairGetX(scrDimensions) - 13) / 2;
    int buttonPosY = pairGetY(scrDimensions) / 2 - 2;

    // Drawning the title
    attrset(A_BOLD | A_BLINK | COLOR_PAIR(1));
    mvaddstr(0, titlePosX, "+----------+");
    mvaddstr(1, titlePosX, "|SNAKE GAME|");
    mvaddstr(2, titlePosX, "+----------+");

    // Drawning the buttons
    attrset(A_BOLD | COLOR_PAIR(3));
    mvaddstr(buttonPosY, buttonPosX, txtButton1);
    attrset(A_BOLD | COLOR_PAIR(2));
    mvaddstr(buttonPosY + 2, buttonPosX, txtButton2);
    mvaddstr(buttonPosY + 4, buttonPosX, txtButton3);
    
    // Moving the cursor back to the first button's line
    move(buttonPosY, buttonPosX);
    refresh();
    return;
}

PAIR *initialize() {
    int maxx, maxy;
    PAIR *scrDimensions;

    initscr();
    noecho();
    cbreak();
    timeout(0);
    curs_set(0);
    start_color();

    // Retrieving the maximum size of our screen
    getmaxyx(stdscr, maxy, maxx);
    scrDimensions = pairCreate(maxx, maxy);

    // Setting all the color pairs used in the game
    init_pair(1, COLOR_BLUE, COLOR_BLACK);    // TITLE
    init_pair(2, COLOR_WHITE, COLOR_BLACK);   // NORMAL TEXTS
    init_pair(3, COLOR_BLACK, COLOR_WHITE);   // MENU BUTTONS
    init_pair(4, COLOR_GREEN, COLOR_BLACK);   // SNAKE
    init_pair(5, COLOR_RED, COLOR_BLACK);     // HEALTHY FRUITS
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK); // UNHEALTHY FRUITS

    drawnMenuScreen(scrDimensions);
    return scrDimensions;
}

// MUDARR
// UI function that moves the cursor up in the list of menu options, changing
// it's colors. the screen dimensions are passed to calculate the y value
void moveCursorUp(PAIR *scrDimensions) {
    int x, y;
    int sButton = pairGetY(scrDimensions) / 2;
    int tButton = sButton + 2;

    getyx(stdscr, y, x);
    
    // There's no need to verify the first button, since it's the highest
    if(y == sButton) {
        attrset(A_BOLD | COLOR_PAIR(2));
        addstr(txtButton2);

        attrset(A_BOLD | COLOR_PAIR(3));
        mvaddstr(y - 2, x, txtButton1);
        move(y - 2, x);
        refresh();

    }
    else if(y == tButton) {
        attrset(A_BOLD | COLOR_PAIR(2));
        addstr(txtButton3);

        attrset(A_BOLD | COLOR_PAIR(3));
        mvaddstr(y - 2, x, txtButton2);
        move(y - 2, x);
        refresh();
    }
    return;
}

void moveCursorDown(PAIR *scrDimensions) {
    int x, y;
    int sButton = pairGetY(scrDimensions) / 2;
    int fButton = sButton - 2;

    getyx(stdscr, y, x);
    
    // There's no need to verify the first button, since it's the highest
    if(y == fButton) {
        attrset(A_BOLD | COLOR_PAIR(2));
        addstr(txtButton1);

        attrset(A_BOLD | COLOR_PAIR(3));
        mvaddstr(y + 2, x, txtButton2);
        move(y + 2, x);
        refresh();

    }
    else if(y == sButton) {
        attrset(A_BOLD | COLOR_PAIR(2));
        addstr(txtButton2);

        attrset(A_BOLD | COLOR_PAIR(3));
        mvaddstr(y + 2, x, txtButton3);
        move(y + 2, x);
        refresh();
    }
    return;
}

int enterOption(PAIR *scrDimensions) {
    int x, y;
    int sButton = pairGetY(scrDimensions) / 2;
    int fButton = sButton - 2;
    int tButton = sButton + 2;

    getyx(stdscr, y, x);
    if(y == fButton)
        return 1;
    else if(y == sButton)
        return 2;
    else
        return 3;

}

WINDOW *renderGameScreen(PAIR *scrDimensions) {
    WINDOW *wind;

    // Drawning the score
    clear();
    mvaddstr(0, (pairGetX(scrDimensions) - 18) / 2, txtScore);
    refresh();

    wind = newwin(25, 50, (pairGetY(scrDimensions) - 25) / 2, (pairGetX(scrDimensions) - 50) / 2);
    //wborder(wind, '|', '|', "-", '-', '+', '+', '+', '+');
    box(wind, 0, 0);
    wrefresh(wind);
    return wind;
}

// Manages the main loop of gameplay for all gameModes
int gameLoop(LEVEL *level) {
    // the loop ends when the player dies or complete the level
    while(!levelEnded(level)) {
        levelGetUserInput(level);
        levelHandleColisions(level);
        napms(100);
    }

    mvaddstr(0, 0, "AAAAAAAAAAAAAAAH");
    refresh();
    napms(500);

    // Verifying if the level ended with a death or completed
    if(levelWon(level)) {
        mvaddstr(3, 0, "NAO");
        refresh();
        napms(500);
        return 5;
    }
    else {
        mvaddstr(2,0, "resetou....");
        refresh();
        napms(500);
        return 4;
    }
}

/*int gameLoop2(LEVEL *level) {
    // the loop ends when the player dies or complete the level
    while(!levelEnded(level)) {

        levelGetUserInput(level);
        levelHandleColisions(level);

        if(levelGetSequence(level)) % 5 == 0)
            levelCreateGoldenFruit(level);

        napms(500);
    }

    // Verifying if the level ended with a death or completed
    if(levelW0n(level))
        return 5;
    else
        return 4;
}*/

int main() {
    LEVEL *level;
    WINDOW *gameWindow;     // Keeps the ncurses' window used in a game loop
    int globalScore = 0;    // Tracks the score of the player in case he wins
    PAIR *scrDimensions;    // Keeps the maximum width and heigth of the terminal opened
    char opCode;            // Used to handle the user input for menu's navigation
    int menuOption = 0;     // Used to handle the option of the menu choosed
    bool running = true;    // Used to verify if the game has ended or not

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
                menuOption = gameLoop(level);
                break;

            case 2:    // Game mode 2 entered
                gameWindow = renderGameScreen(scrDimensions);
                //level = levelCreate();
                //menuOption = gameLoop(level);
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