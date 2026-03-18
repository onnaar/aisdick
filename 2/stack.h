#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdbool.h>

typedef enum {
    MEMORY_ERROR = -1,
    NULLPTR,
    OK,
    STACK_OVERFLOW,
    STACK_EMPTY,
    WRONG_EXPRESSION
} status;

typedef struct Stack Stack;

Stack *StackCreate();
status StackPush(Stack *, const char);
char StackPop(Stack *);
void StackOutput(Stack *);
void StackFakeOutput(const Stack *);
void StackFree(Stack *);
void StackClean(Stack *);
bool IsEmpty(Stack *);
bool IsFull(Stack *);

#endif 
