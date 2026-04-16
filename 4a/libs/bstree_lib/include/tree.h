#ifndef TREE_H
#define TREE_H

#include "node.h"

typedef enum {
    OK = 0,
    NOT_EXIST,
    MEMORY_ERROR,
    NOT_FOUND,
    TREE_EMPTY
} TreeStatus;

typedef struct {
    Node *root;
} Tree;

Tree *TreeCreate(size_t key, size_t *value);
TreeStatus TreeInsert(Tree *tree, size_t key, size_t *value);
TreeStatus TreeKeyDelete(Tree *tree, size_t key);
Node *FindKey(Tree *tree, size_t key);
Node *FindMinKey(Node *node);
Node *FindNextKey(Tree *tree, size_t key);
TreeStatus TreeOutput(Tree *tree, char type);

void TreeDelete(Tree *tree);

#endif
