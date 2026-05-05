#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

typedef enum {
    STACK_OK = 0,
    STACK_NULLPTR,
    STACK_MEMORY_ERROR,
    STACK_EMPTY
} StackStatus;

typedef struct Stack Stack;

Stack *StackCreate();

StackStatus StackPush(Stack *const stack, void *const data);
void *StackPop(Stack *const stack);
StackStatus StackMemoryManage(Stack *const stack);
bool IsEmpty(const Stack *const stack);

void StackFree(Stack *stack);

#endif
