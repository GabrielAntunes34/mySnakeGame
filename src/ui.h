#ifndef UI_H
    #define UI_H
    #include "level.h"
    #include <ncurses.h>


    #define txtButton1 "   Classic   "
    #define txtButton2 "Rotten fruits"
    #define txtButton3 "  Quit game  "
    #define txtScore "SCORE: "

    #define gameWidth 50
    #define gameHeight 25



    // Functions to interact with ncurses
    PAIR *initialize();
    void moveCursorUp(PAIR *scrDimensions);
    void moveCursorDown(PAIR *scrDimensions);
    int enterOption(PAIR *scrDimensions);

    // Functions to drawn and render things
    WINDOW *renderGameScreen(PAIR *scrDimensions);
    void drawnScore(LEVEL *level);
    void drawnMenuScreen(PAIR *scrDimensions);
    void drawnFruit(WINDOW *win, FRUIT *fruit);
    void firstDrawnSnake(WINDOW *win, PAIR *pos, int size);
    void drawnSnake(WINDOW *win, PAIR *tail, PAIR *head, PAIR *newHead);

#endif