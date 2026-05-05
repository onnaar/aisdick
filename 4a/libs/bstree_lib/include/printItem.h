#ifndef PRINT_ITEM_H
#define PRINT_ITEM_H

#define MAX_PRINT_DEPTH 255

#include <stdbool.h>
#include "node.h"

typedef struct {
    Node *node;
    size_t depth;
    RelativeIndex index;
    bool is_last_child;
    bool level_history[MAX_PRINT_DEPTH]; 
} PrintStackItem;

PrintStackItem *PrintStackItemCreate();

void PrintStackItemDelete(PrintStackItem *cur);

#endif
