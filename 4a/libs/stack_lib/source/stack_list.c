#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"

typedef struct NodeStack {
    struct NodeStack *next;
    void *data;
} NodeStack;

struct Stack {
    NodeStack *top;
};

Stack *StackCreate() {
    return (Stack *)calloc(1, sizeof(Stack));
}

StackStatus StackPush(Stack *const stack, void *data) {
    if (!stack) {
        return STACK_NULLPTR;
    }
    NodeStack *new_node = (NodeStack *)calloc(1, sizeof(NodeStack));
    if (!new_node) {
        return STACK_MEMORY_ERROR;
    }
    new_node->data = data;
    new_node->next = stack->top;
    stack->top = new_node;
    return STACK_OK;
}

void *StackPop(Stack *const stack) {
    if (!stack || !stack->top) {
        return NULL;
    }
    NodeStack *temp = stack->top;
    void *data = temp->data;
    stack->top = temp->next;
    free(temp);
    return data;
}

void StackFree(Stack *stack) {
    if (!stack) {
        return;
    }
    while (stack->top) {
        StackPop(stack);
    }
    free(stack);
}

bool IsEmpty(const Stack *const stack) {
    return (stack == NULL || stack->top == NULL);
}
