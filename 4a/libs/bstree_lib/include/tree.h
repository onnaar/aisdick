#ifndef TREE_H
#define TREE_H

#include "nodeArray.h"

typedef enum {
    TREE_OK = 0,
    TREE_NOT_VALID,
    TREE_MEMORY_ERROR,
    TREE_NOT_FOUND,
    TREE_EMPTY,
    TREE_END,
    TREE_WRONG_FORMAT,
    TREE_DUPLICATE
} TreeStatus;

typedef struct {
    Node *root;
} Tree;

Tree *TreeCreate();

TreeStatus TreeInsert(Tree *const tree, const size_t key, const NodeInfo *const value);

NodeArray *FindKey(const Tree *const tree, const size_t key);
Node *FindKeyRelease(const Tree *const tree, const size_t key, const size_t release);

TreeStatus TreeKeyDelete(Tree *const tree, const size_t key);

Node *FindMinKey(Node *node);
Node *FindNextKey(const Tree *const tree, const size_t key);

void AllSpecialNodes(Node *cur, void *context);
void Delete(Node *cur, void *context);
void Special(Node *cur, void *context);
void Output (Node *cur, void *context);

TreeStatus TreeTraversing(Tree *tree, void (*action)(Node *cur, void *context), void *context);
SpSearchStructure *SpecialSearch(Tree *const tree, const size_t key);

TreeStatus TreeImport(Tree *const tree, const char *const filename);
TreeStatus TreeExport(const Tree *tree, const char *const filename);
TreeStatus TreeExportDot(const Tree *const tree, const char *const filename);

TreeStatus TreeOutput(const Tree *const tree);

#endif
