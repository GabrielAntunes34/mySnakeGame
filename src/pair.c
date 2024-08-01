#include <stdio.h>
#include <stdlib.h>
#include "pair.h"

struct pair_int {
    int x;
    int y;
};

PAIR *pairCreate(int x, int y) {
    PAIR *pair;

    pair = (PAIR *) malloc(sizeof(PAIR));
    if(pair != NULL) {
        pair->x = x;
        pair->y = y;
    }

    return pair;
}


bool pairSetX(PAIR *pair, int x) {
    if(pair == NULL)
        return false;

    pair->x = x;
    return true;
}

bool pairSetY(PAIR *pair, int y) {
    if(pair == NULL)
        return false;
    
    pair->y = y;
    return true;
}

bool pairSetxy(PAIR *pair, int x, int y) {
    if(pair == NULL)
        return false;

    pair->x = x;
    pair->y = y;
    return true;
}


int pairGetX(PAIR *pair) {
    return ((pair != NULL) ? pair->x : PAIR_ERROR);
}

int pairGetY(PAIR *pair) {
    return ((pair != NULL) ? pair->y : PAIR_ERROR);
}

// Compares the pair members directly, and if both are equal, returns true.
bool pairCompare(PAIR *p1, PAIR *p2) {
    if(p1 != NULL && p2 != NULL) {
        if(p1->x == p2->x && p1->y == p2->y)
            return true;
    }
    return false;
}

PAIR *pairCopy(PAIR *pair) {
    if(pair == NULL)
        return false;
    PAIR *newPair = pairCreate(pairGetX(pair), pairGetY(pair));
    return newPair;
}

bool pairIncrementX(PAIR *pair) {
    if(pair == NULL)
        return false;
    
    pair->x++;
    return true;
}

bool pairIncrementY(PAIR *pair) {
    if(pair == NULL)
            return false;
        
        pair->y++;
        return true;
}

bool pairDecrementX(PAIR *pair) {
    if(pair == NULL)
        return false;
    
    pair->x--;
    return true;
}

bool pairDecrementY(PAIR *pair) {
    if(pair == NULL)
        return false;
    
    pair->y--;
    return true;
}

void pairPrint(PAIR *pair) {
    if(pair != NULL)
        printf("(%d, %d)\n", pair->x, pair->y);
}

bool pairDelete(PAIR **pair) {
    if(*pair == NULL)
        return false;

    free(*pair);
    *pair = NULL;
    return true;
}
