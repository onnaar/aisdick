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
    size_t info; 
    size_t max_delta;
    NodeArray *array;
} SpSearchStructure;

typedef struct {
    Node *root;
} Tree;

Tree *TreeCreate(void);

TreeStatus TreeInsert(Tree *tree, size_t key, size_t *value);
TreeStatus TreeKeyDelete(Tree *tree, size_t key);
Node *FindKey(Tree *tree, size_t key);
NodeArray *FindKeyRelease(Tree *tree, size_t key, size_t release);
void Output(Node *cur, void *context);
void NodeArrayAdd(NodeArray *array, Node *node);
void AllSpecialNodes(Node *cur, void *context);
void Special(Node *cur, void *context);
TreeStatus TreeTraversing(Tree *tree, void (*action)(Node *cur, void *context), void *context);
SpSearchStructure *SpecialSearch(Tree *tree, size_t info);
Node *FindMinKey(Node *node);
Node *FindNextKey(Tree *tree, size_t key);

void SpSearchStructureDelete(SpSearchStructure *data);
void TreeDelete(Tree *tree);

#endif
