#ifndef FRUIT_H
    #define FRUIT_H
    #include <stdbool.h>
    #include "pair.h"

    #define FRUIT_SPRITE 'F'
    #define FRUIT_DF_VALUE 25
    typedef struct fruit FRUIT;

    FRUIT *fruitCreate(PAIR *pos, bool healthiness);

    // Getters
    PAIR *fruitGetPosition(FRUIT *fruit);
    int fruitGetValue(FRUIT *fruit);
    bool fruitIsTangible(FRUIT *fruit);
    bool fruitIsHealthy(FRUIT *fruit);

    // Setters
    bool fruitSetPosition(FRUIT *fruit, PAIR *newPos);
    bool fruitSetValue(FRUIT *fruit, int newValue);
    bool fruitInvertTangibility(FRUIT *fruit);
    bool fruitInvertyHealthiness(FRUIT *fruit);
    
    bool fruitDelete(FRUIT **fruit);

#endif