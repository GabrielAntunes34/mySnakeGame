#ifndef PAIR_H
    #define PAIR_H
    #include <stdbool.h>
    
    typedef struct pair_int PAIR;
    #define PAIR_ERROR -3200

    PAIR *pairCreate(int x, int y);

    // Setters
    bool pairSetX(PAIR *pair, int x);
    bool pairSetY(PAIR *pair, int y);
    bool pairSetxy(PAIR *pair, int x, int y);

    // Getters
    int pairGetX(PAIR *pair);
    int pairGetY(PAIR *pair);

    // Operations with pairs
    bool pairCompare(PAIR *p1, PAIR *p2);
    PAIR *pairCopy(PAIR *pair);
    bool pairIncrementX(PAIR *pair);
    bool pairIncrementY(PAIR *pair);
    bool pairDecrementX(PAIR *pair);
    bool pairDecrementY(PAIR *pair);
    void pairPrint(PAIR *pair);

    bool pairDelete(PAIR **pair);
#endif