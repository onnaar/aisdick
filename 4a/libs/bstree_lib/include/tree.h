#ifndef TREE_H
#define TREE_H

#define MAX_PRINT_DEPTH 128

#include <stdbool.h>
#include "node.h"

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
    Node *node;
    size_t depth;
    bool line_history[MAX_PRINT_DEPTH]; 
    bool is_last_child;
} PrintStackItem;

typedef struct {
    size_t info; 
    size_t max_delta;
    NodeArray *array;
} SpSearchStructure;

typedef struct {
    Node *root;
} Tree;

Tree *TreeCreate();

TreeStatus TreeInsert(Tree *tree, size_t key, size_t *value);
TreeStatus TreeKeyDelete(Tree *tree, size_t key);
Node *FindKey(Tree *tree, size_t key);
NodeArray *FindKeyRelease(Tree *tree, size_t key, size_t release);
void Output(Node *cur, void *context);
void NodeArrayAdd(NodeArray *array, Node *node);
void AllSpecialNodes(Node *cur, void *context);
void Delete(Node *cur, void *context);
void Special(Node *cur, void *context);
TreeStatus TreeTraversing(Tree *tree, void (*action)(Node *cur, void *context), void *context);
SpSearchStructure *SpecialSearch(Tree *tree, size_t info);
TreeStatus TreeImport(Tree *const tree, const char *const filename);
Node *FindMinKey(Node *node);
Node *FindNextKey(Tree *tree, size_t key);
TreeStatus TreeOutput(Tree *tree);

void SpSearchStructureDelete(SpSearchStructure *data);
//void TreeDelete(Tree *tree);

TreeStatus TreeExport(Tree *tree, const char *filename);
TreeStatus TreeGraphviz(Tree *tree, const char *filename);

#endif
