#ifndef TREE_H
#define TREE_H

#include "node.h"

typedef enum {
    TREE_OK = 0,
    TREE_NOT_EXIST,
    TREE_MEMORY_ERROR,
    TREE_NOT_FOUND,
    TREE_EMPTY
} TreeStatus;

typedef struct {
    Node *root;
} Tree;

Tree *TreeCreate(size_t key, size_t *value);
TreeStatus TreeInsert(Tree *tree, size_t key, size_t *value);
TreeStatus TreeKeyDelete(Tree *tree, size_t key);
Node *FindKey(Tree *tree, size_t key);
NodeArray *FindKeyRelease(Tree *tree, size_t key, size_t release);
NodeArray *SpecialSearch(Tree *tree, size_t info);
Node *FindMinKey(Node *node);
Node *FindNextKey(Tree *tree, size_t key);
TreeStatus TreeOutput(Tree *tree);

void TreeDelete(Tree *tree);

#endif
