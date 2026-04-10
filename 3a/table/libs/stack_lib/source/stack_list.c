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

StackStatus StackPush(Stack *stack, const Node *node) {
    if (!stack) {
        return NULLPTR;
    }
    NodeStack *cur_node = (NodeStack *)calloc(1, sizeof(NodeStack));
    if (!cur_node) {
        return MEMORY_ERROR;
    }
    cur_node->node = NodeCopy(node);
    if (!cur_node->node) {
        return MEMORY_ERROR;
    }
    if (!stack->top) {
        stack->top = cur_node;
    } else {
        cur_node->next = stack->top;
        stack->top = cur_node;
    }
    return OK;
}

Node *StackPop(Stack *stack) {
    if (!stack) {
        return NULL;
    }
    if (IsEmpty(stack)) {
        return NULL;
    }
    NodeStack *temp = stack->top;
    Node *cur_node = NodeCopy(temp->node);
    if (!cur_node) {
        return NULL;
    }
    stack->top = temp->next;
    NodeDelete(temp->node);
    free(temp);
    return cur_node;
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
