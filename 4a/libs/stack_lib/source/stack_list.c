#include <stdlib.h>
#include "node.h"
#include "stack.h"

typedef struct NodeStack {
    struct NodeStack *next;
    Node *node;
} NodeStack;

struct Stack {
    NodeStack *top;
};

Stack *StackCreate() {
    return (Stack *)calloc(1, sizeof(Stack));
}

StackStatus StackPush(Stack *const stack, Node *node) {
    if (!stack) {
        return STACK_NULLPTR;
    }
    NodeStack *cur_node = (NodeStack *)calloc(1, sizeof(NodeStack));
    if (!cur_node) {
        return STACK_MEMORY_ERROR;
    }
    cur_node->node = node;
    if (!stack->top) {
        stack->top = cur_node;
    } else {
        cur_node->next = stack->top;
        stack->top = cur_node;
    }
    return STACK_OK;
}

Node *StackPop(Stack *const stack) {
    if (!stack) {
        return NULL;
    }
    if (IsEmpty(stack)) {
        return NULL;
    }
    NodeStack *temp = stack->top;
    Node *temp_node = temp->node;
    stack->top = temp->next;
    free(temp);
    return temp_node;
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
    return (!stack->top);
}
