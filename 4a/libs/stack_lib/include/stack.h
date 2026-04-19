#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include "node.h"

typedef enum {
    STACK_MEMORY_ERROR = -1,
    STACK_NULLPTR,
    STACK_OK,
    STACK_OVERFLOW,
    STACK_EMPTY,
} StackStatus;

typedef struct Stack Stack;

Stack *StackCreate();
StackStatus StackPush(Stack *const, Node *);
Node *StackPop(Stack *const);
void StackFree(Stack *stack);
bool IsEmpty(const Stack *const);

#endif 
