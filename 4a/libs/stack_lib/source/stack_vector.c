#include <stdlib.h>
#include "stack.h"

#define SIZE 1

struct Stack{
    void **data;
    size_t size;
    size_t top;
};

Stack *StackCreate() {
    Stack *stack = (Stack *)calloc(1, sizeof(Stack));
    if (!stack) {
        return NULL;
    }
    stack->data = (void *)calloc(SIZE, sizeof(void *));
    if (!stack->data) {
        return NULL;
    }
    stack->size = SIZE;
    return stack;
}

StackStatus StackMemoryManage(Stack *const stack) {
    if (!stack) {
        return STACK_NULLPTR;
    }
    stack->size *= 2;
    void **new_data = (void **)realloc(stack->data, stack->size * sizeof(void *));
    if (!new_data) {
        return STACK_MEMORY_ERROR;
    }
    stack->data = new_data;
    return STACK_OK;
}

StackStatus StackPush(Stack *const stack, void *const data) {
    if (!stack) {
        return STACK_NULLPTR;
    }
    if (stack->top == stack->size) {
        StackStatus status = StackMemoryManage(stack);
        if (status != STACK_OK) {
            return status;
        }
    }
    stack->data[stack->top++] = data;
    return STACK_OK;
}

void *StackPop(Stack *const stack) {
    if (!stack) {
        return NULL;
    }
    if (IsEmpty(stack)) {
        return NULL;
    }
    return stack->data[--(stack->top)];
}

void StackFree(Stack *stack) {
    if (!stack) {
        return;
    }
    free(stack->data);
    free(stack);
}

bool IsEmpty(const Stack *const stack) {
    return stack->top == 0;
}
