#include <stdlib.h>
#include "stack.h"

typedef struct NodeStack {
    struct NodeStack *next;
    int value;
} NodeStack;

struct Stack {
    NodeStack *top;
};

Stack *StackCreate() {
    return (Stack *)calloc(1, sizeof(Stack));
}

status StackPush(Stack *stack, const char value) {
    if (!stack) {
        return NULLPTR;
    }
    NodeStack *cur_node = (NodeStack *)calloc(1, sizeof(NodeStack));
    if (!cur_node) {
        return MEMORY_ERROR;
    }
    if (!stack->top) {
        stack->top = cur_node;
    } else {
        cur_node->next = stack->top;
        stack->top = cur_node;
    }
    cur_node->value = value;
    return OK;
}

char StackPop(Stack *stack) {
    if (!stack) {
        return NULLPTR;
    }
    if (IsEmpty(stack)) {
        return STACK_EMPTY;
    }
    NodeStack *temp = stack->top;
    char value = temp->value;
    stack->top = temp->next;
    free(temp);
    return value;
}

void StackOutput(Stack *stack) {
    if (!stack) {
        return;
    }
    printf("\"");
    while (stack->top) {
        printf("%c", StackPop(stack));
    }
    printf("\"\n");
}

void StackFakeOutput(const Stack *stack) {
    if (!stack) {
        return;
    }
    NodeStack *cur = stack->top;
    printf("\"");
    while (cur) {
        printf("%c", cur->value);
        cur = cur->next;
    }
    printf("\"\n");
}
 

void StackClean(Stack *stack) {
    if (!stack) {
        return;
    }
    while (stack->top) {
        StackPop(stack);
    }
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

bool IsEmpty(Stack *stack) {
    if (!stack->top) {
        return true;
    }
    return false;
}
