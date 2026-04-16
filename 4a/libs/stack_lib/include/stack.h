#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include "node.h"

typedef enum {
    MEMORY_ERROR = -1,
    NULLPTR,
    OK,
    STACK_OVERFLOW,
    STACK_EMPTY,
} StackStatus;

typedef struct Stack Stack;

Stack *StackCreate();
StackStatus StackPush(Stack *, const Node *);
Node *StackPop(Stack *);
void StackFree(Stack *stack);
bool IsEmpty(Stack *);

#endif 
