#include <stdlib.h>
#include "snake.h"

typedef struct snakeNode SNAKENODE;

struct snake {
    SNAKENODE *head;
    SNAKENODE *tail;
    int size;
    bool tangible;
};

struct snakeNode {
    PAIR *pos;
    SNAKENODE *previous;
};


// Auxiliar function that creates a new snake node.
SNAKENODE *newSnakeNode(PAIR *position) {
    SNAKENODE *node;

    node = (SNAKENODE *) malloc(sizeof(SNAKENODE));
    if(node == NULL)
        return NULL;

    node->pos = position;
    node->previous = NULL;
    return node;
}

SNAKE *snakeCreate(PAIR *position, int initSize) {
    SNAKE *snake;

    // Allocating the snake.
    snake = (SNAKE *) malloc(sizeof(SNAKE));
    if(snake == NULL)
        return NULL;

    // Allocating the head with it's initial position
    
    snake->head = newSnakeNode(pairCopy(position));
    if(snake->head == NULL) {
        free(snake);
        snake = NULL;
        return NULL;
    }
    snake->size = 1;
    snake->tangible = true;
    snake->tail = snake->head;

    // Givving the initial size of this instance
    // This also will set the tail's pointer accordingly
    while(snake->size < initSize)
        snakeIncrease(snake);
    return snake;
}


PAIR *snakeGetPosition(SNAKE *snake) {
    return ((snake != NULL) ? snake->head->pos : NULL);
}

PAIR *snakeGetTail(SNAKE *snake) {
    return ((snake != NULL) ? snake->tail->pos : NULL);
}

int snakeGetSize(SNAKE *snake) {
    return ((snake != NULL) ? snake->size : 0);
}

bool snakeIsTangible(SNAKE *snake) {
    return ((snake != NULL) ? snake->tangible : false);
}


// Abilitates or disabilitates the colisions of a SNAKE by negating it's previous state.
bool snakeInvertTangibility(SNAKE *snake) {
    if(snake != NULL) {
        snake->tangible = !snake->tangible;
        return true;
    }
    return false;
}

// Adds a new node to the snake's tail, uptating it's pointers and size.
bool snakeIncrease(SNAKE *snake) {
    if(snake == NULL)
        return false;
    
    SNAKENODE *newNode;

    // Creating the new node behind the tail
    newNode = newSnakeNode(pairCopy(snake->tail->pos));
    if(newNode == NULL)
        return false;
    pairDecrementX(newNode->pos);
    
    // Inserting it into the snake's linked list
    snake->tail->previous = newNode;
    snake->tail = newNode;

    // Special case for the first increase
    if(snake->size == 1)
        snake->head->previous = newNode;

    snake->size++;
    return true;
}

// Recieves a newPosition to the snake's head and increments it while adjusting the
// rest of the snake's linked list
bool snakeMove(SNAKE *snake, PAIR *newHeadPosition) {
    if(snake == NULL || newHeadPosition == NULL)
        return false;
    SNAKENODE *ptr;     // Pointer used to run trougth the snake
    PAIR *previousSave; // Saves the previous pointer position for each iteration
    PAIR *aux;         // Auxiliar to change the positions

    // Updating the position of every node
    previousSave = snake->head->pos;
    for(ptr = snake->head; ptr->previous != NULL; ptr = ptr->previous) {
        aux = previousSave;
        previousSave = ptr->previous->pos;
        ptr->previous->pos = aux;
    }

    // Deleting the last position's pair
    pairDelete(&previousSave);

    // Updating the head Position
    snake->head->pos = pairCopy(newHeadPosition);
    return true;
}

// Verifies if the snake colided with any of it's inner nodes.
bool snakeInnerColision(SNAKE *snake) {
    if(snake == NULL || snake->tangible == false || snake->size < 4)
        return false;

    SNAKENODE *aux;     // Pointer used to run trougth the snake

    // Comparing the head's position with every snake node.
    for(aux = snake->head->previous; aux != NULL; aux = aux->previous) {
        if(pairCompare(snake->head->pos, aux->pos))
            return true;
    }
    return false;
}

// Resets the snake size to the standards providade
void snakeReset(SNAKE **snake, PAIR * stdPos, int stdSize) {
    if(*snake != NULL && stdPos != NULL) {
        snakeDelete(snake);
        *snake = snakeCreate(stdPos, stdSize);
    }
}

// Desallocs a snake, it's nodes and pairs form memory
bool snakeDelete(SNAKE **snake) {
    if(*snake != NULL) {
        SNAKENODE *ptr;     // Pointer used to run trougth the snake
        SNAKENODE *aux;     // Pointer used to free all nodes allocated

        // Desallocating all nodes and pairs
        ptr = (*snake)->head;
        while(ptr != NULL) {
            aux = ptr;
            ptr = ptr->previous;

            pairDelete(&aux->pos);
            free(aux);
        }

        //adjusting the pointers used
        ptr = NULL;
        (*snake)->head = NULL;
        (*snake)->tail = NULL;

        // Desallocating the snake
        free(*snake);
        *snake = NULL;
        return true;
    }
    return false;
}
