#include <stdlib.h>
#include <ncurses.h>
#include "ui.h"

// Auxiliar to move the cursor and print inside an especific window
void wmvaddch(WINDOW *win, int y, int x, char c) {
    wmove(win, y, x);
    waddch(win, c);
}


// Initializes ncurses in the program, calling the first drawn of the menu screen
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
    init_pair(7, COLOR_YELLOW, COLOR_BLACK);  // GOLDEN FRUITS

    drawnMenuScreen(scrDimensions);
    return scrDimensions;
}


//!!!!!!!!!! MUDAR !!!!!!!!!!!!!!!
// INTERACTION
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

// Enters an option in the menu
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


///////////////////////
// DRAWINNG FUNCTIONS//
///////////////////////

// Generates the game window with the box drawning where the entire level
// will be instanciated
WINDOW *renderGameScreen(PAIR *scrDimensions) {
    WINDOW *wind;

    // Cleareing the previous screen
    clear();
    refresh();

    wind = newwin(25, 50, (pairGetY(scrDimensions) - 25) / 2, (pairGetX(scrDimensions) - 50) / 2);
    //wborder(wind, '|', '|', "-", '-', '+', '+', '+', '+');
    box(wind, 0, 0);
    wrefresh(wind);
    return wind;
}

void drawnScore(LEVEL *level) {
    int x;      // used to calculate the position initial x position in the screen

    // Calculating the x position to drawn the score
    x = getmaxx(stdscr);
    x = (x - 18) / 2;

    // Drawning a yellow scodre
    attrset(A_BOLD | COLOR_PAIR(7));
    move(0, x);
    printw(txtScore);
    printw("%011d", levelGetScore(level));
    refresh();
    return;
}

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

// Drawn any fruit, healthy or rotten, into the game screnn
void drawnFruit(WINDOW *win, FRUIT *fruit) {
    if(fruit != NULL) {
        // Setting it's collor acordingly
        if(fruitIsHealthy(fruit) && fruitGetValue(fruit) == FRUIT_DF_VALUE)
            wattrset(win, COLOR_PAIR(5));
        else if(!fruitIsHealthy(fruit))
            wattrset(win, COLOR_PAIR(6));
        else
            wattrset(win, COLOR_PAIR(7));
        
        // Drwining it in the given position
        wmvaddch(win, pairGetY(fruitGetPosition(fruit)), pairGetX(fruitGetPosition(fruit)), FRUIT_SPRITE);
    }
}

// Used to drawn the snake with all it's nodes in the first renderization
// of the current level
void firstDrawnSnake(WINDOW *win, PAIR *pos, int size) {
    int x = pairGetX(pos);
    int y = pairGetY(pos);

    wattrset(win, COLOR_PAIR(4) | A_BOLD);
    wmvaddch(win, y, x, SNAKE_HEAD);
    for(int i = size; i > 1; i--) {
        x--;
        wmvaddch(win, y, x, SNAKE_BODY);
    }

    wrefresh(win);
    return;
}

// Redraws the snake as it moves, only changing the head, to put it in the next
// position and erasing the previous tail
void drawnSnake(WINDOW *win, PAIR *tail, PAIR *head, PAIR *newHead) {
    wattrset(win, COLOR_PAIR(4));

    wmvaddch(win, pairGetY(tail), pairGetX(tail), ' ');
    wmvaddch(win, pairGetY(newHead), pairGetX(newHead), SNAKE_HEAD);
    wmvaddch(win, pairGetY(head), pairGetX(head), SNAKE_BODY);
    wrefresh(win);
}