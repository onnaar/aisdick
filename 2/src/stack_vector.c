#include <stddef.h>
#include <stdlib.h>
#include "stack.h"

struct Stack{
    char *data;
    size_t size;
    size_t top;
};

Stack *StackCreate(const size_t size) {
    Stack *stack = (Stack *)calloc(1, sizeof(Stack));
    if (!stack) {
        return NULL;
    }
    stack->data = (char *)calloc(size, sizeof(char));
    if (!stack->data) {
        return NULL;
    }
    stack->size = size;
    return stack;
}

status StackPush(Stack *const stack, const char value) {
    if (!stack) {
        return NULLPTR;
    }
    if (IsFull(stack)) {
        return STACK_OVERFLOW;
    }
    stack->data[stack->top++] = value;
    return OK;
}


char StackPop(Stack *const stack) {
    if (!stack) {
        return NULLPTR;
    }
    if (IsEmpty(stack)) {
        return STACK_EMPTY;
    }
    return stack->data[--(stack->top)];
}

void StackOutput(Stack *const stack) {
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

void StackFakeOutput(const Stack *const stack) {
    if (!stack) {
        return;
    }
    printf("\"");
    for (size_t i = 0; i < stack->top; i++) {
        printf("%c", stack->data[stack->top - 1 - i]);
    }
    printf("\"\n");
}

void StackFree(Stack *stack) {
    free(stack->data);
    free(stack);
}

bool IsEmpty(const Stack *const stack) {
    return stack->top == 0;
}

bool IsFull(const Stack *const stack) {
    return stack->top == stack->size;
}

