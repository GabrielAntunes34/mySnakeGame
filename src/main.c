#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "snake.h"
#include "fruit.h"

#define txtButton1 "   Classic   "
#define txtButton2 "Rotten fruits"
#define txtButton3 "  Quit game  "


// MUDAR????
void firstDrawnSnake(PAIR *pos, int size) {
    int x = pairGetX(pos);
    int y = pairGetY(pos);

    move(y, x);
    addch(SNAKE_HEAD);
    for(int i = size; i > 1; i--) {
        x--;
        move(y, x);
        addch(SNAKE_BODY);
    }

    refresh();
    return;
}

// MANTER: FUNCIONA PARA REDESENHÁ-LA SEMPRE
void drawnSnake(PAIR *tail, PAIR *head, PAIR *newHead) {
    mvaddch(pairGetY(tail), pairGetX(tail), ' ');
    mvaddch(pairGetY(newHead), pairGetX(newHead), SNAKE_HEAD);
    mvaddch(pairGetY(head), pairGetX(head), SNAKE_BODY);
    refresh();
}


// UI function to drwan the menu screen of the game
void drawnMenuScreen(PAIR *scrDimensions) {
    // Title length: 10
    int titlePosX  = (pairGetX(scrDimensions) - 10)/ 2;
    int buttonPosX = (pairGetX(scrDimensions) - 13) / 2;
    int buttonPosY = pairGetY(scrDimensions) / 2 - 2;

    // Drawning the title
    attron(A_BOLD | A_BLINK | COLOR_PAIR(1));
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
    curs_set(0);
    start_color();
    noecho();

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

int gameLoop(LEVEL *level) {
    while(!levelEnded(level)) {
        levelGetUserInput();
        
    }
}

int main() {
    LEVEL *level;
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
            case 1:
                renderGameScreen();
                level = levelCreate();
                opCode = gameLoop();

                break;
            case 2:
                renderGameScreen();
                level = levelCreate();
                opCode = gameLoop();

                break;
            case 3:
                running = false;
                break;
            case 4:
                initialize();
            case 5:
                levelReset();
                opCode = gameLoop();
        }
    }

    pairDelete(&scrDimensions);
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
