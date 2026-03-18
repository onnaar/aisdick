#include <stddef.h>
#include <stdlib.h>
#include "stack.h"

#define STACK_SIZE 10

struct Stack{
    char *data;
    size_t size;
    size_t top;
};

Stack *StackCreate() {
    Stack *stack = (Stack *)calloc(1, sizeof(Stack));
    if (!stack) {
        return NULL;
    }
    stack->data = (char *)calloc(STACK_SIZE, sizeof(char));
    if (!stack->data) {
        return NULL;
    }
    stack->size = STACK_SIZE;
    return stack;
}

status StackPush(Stack *stack, const char value) {
    if (!stack) {
        return NULLPTR;
    }
    if (IsFull(stack)) {
        return STACK_OVERFLOW;
    }
    stack->data[stack->top++] = value;
    return OK;
}


char StackPop(Stack *stack) {
    if (!stack) {
        return NULLPTR;
    }
    if (IsEmpty(stack)) {
        return STACK_EMPTY;
    }
    return stack->data[--(stack->top)];
}

void StackOutput(Stack *stack) {
    if (!stack) {
        return;
    }
    char value = 0;
    printf("\"");
    while ((value = StackPop(stack)) != STACK_EMPTY) {
        printf("%c", value);
    }
    printf("\"\n");
}

void StackFakeOutput(const Stack *stack) {
    if (!stack) {
        return;
    }
    printf("\"");
    for (size_t i = 0; i < stack->top; i++) {
        printf("%c", stack->data[stack->top - 1 - i]);
    }
    printf("\"\n");
}

void StackClean(Stack *stack) {
    while (stack->top) {
        StackPop(stack);
    }
}

void StackFree(Stack *stack) {
    free(stack->data);
    free(stack);
}

bool IsEmpty(Stack *stack) {
    if (stack->top == 0) {
        return true;
    }
    return false;
}

bool IsFull(Stack *stack) {
    if (stack->top == stack->size) {
        return true;
    }
    return false;
}

