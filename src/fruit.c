#include <stdlib.h>
#include "fruit.h"

struct fruit {
    PAIR *pos;
    int value;
    bool tangible;
    bool healthy;
};

// Creates a fruit with the default value of 25
FRUIT *fruitCreate(PAIR *pos, bool healthiness) {
    if(pos == NULL)
        return NULL;

    FRUIT *fruit;

    // Allocating and setting the fruit
    fruit = (FRUIT *) malloc(sizeof(FRUIT));
    if(fruit != NULL) {
        fruit->pos = pos;
        fruit->value = FRUIT_DF_VALUE;
        fruit->tangible = true;
        fruit->healthy = healthiness;

        if(!fruit->healthy)
            fruit->value = fruit->value * -1;
    }
    return fruit;
}


PAIR *fruitGetPosition(FRUIT *fruit) {
    return ((fruit != NULL) ? fruit->pos : NULL);
}

int fruitGetValue(FRUIT *fruit) {
    return ((fruit != NULL) ? fruit->value : 0);
}

bool fruitIsTangible(FRUIT *fruit) {
    return ((fruit != NULL) ? fruit->tangible : false);
}

bool fruitIsHealthy(FRUIT *fruit) {
    return ((fruit != NULL) ? fruit->healthy : false);
}


// Sets a new position to the given fruit, desallocating the last one from memory
// It acutaly copies the newPosition to a new pair, but dosen't delete the one passed as argument
bool fruitSetPosition(FRUIT *fruit, PAIR *newPos) {
    if(fruit == NULL)
        return false;

    pairDelete(&fruit->pos);
    fruit->pos = pairCopy(newPos);
    return true;
}

// Sets a new value to the given fruit
bool fruitSetValue(FRUIT *fruit, int newValue) {
    if(fruit == NULL)
        return false;

    fruit->value = newValue;
    return true;
}

// Negates the current tangible state of the given fruit
bool fruitInvertTangibility(FRUIT *fruit) {
    if(fruit == NULL)
        return false;

    fruit->tangible = !fruit->tangible;
    return true;
}

// Negates the current healty state of the given fruit
bool fruitInvertyHealthiness(FRUIT *fruit) {
    if(fruit == NULL)
        return false;

    fruit->healthy = !(fruit->healthy);
    return true;
}

// Desalocates a fruit from the memory and it's position's pair.
bool fruitDelete(FRUIT **fruit) {
    if(*fruit == NULL)
        return false;

    pairDelete(&((*fruit)->pos));
    free(*fruit);
    *fruit = NULL;
    return true;
}