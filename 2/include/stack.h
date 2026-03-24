#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdbool.h>

typedef enum {
    MEMORY_ERROR = 0,
    NULLPTR,
    OK,
    STACK_OVERFLOW,
    STACK_EMPTY,
    WRONG_EXPRESSION
} status;

typedef struct Stack Stack;

Stack *StackCreate(const size_t);
status StackPush(Stack *const, const char);
char StackPop(Stack *const);
void StackOutput(Stack *const);
void StackFakeOutput(const Stack *const);
void StackFree(Stack *);
bool IsEmpty(const Stack *const);
bool IsFull(const Stack *const);

#endif 
