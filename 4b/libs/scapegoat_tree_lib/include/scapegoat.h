#ifndef SCAPEGOAT_H
#define SCAPEGOAT_H

#include <stdbool.h>
#include <stdio.h>
#include "vector.h"

typedef int (*CompareFunc)(const void *const key1, const void *const key2);
typedef void (*DestroyFunc)(void *const data);

typedef enum {
    LEFT = 0,
    RIGHT = 1
} SiblingIndex;

typedef enum {
    TREE_OK = 0,
    TREE_NOT_VALID,
    TREE_NOT_FOUND,
    TREE_MEMORY_ERROR,
    TREE_DUPLICATE
} TreeStatus;

typedef struct Node {
    struct Node *relatives[2]; 
    void *key;
    void *value;
} Node;

typedef struct {
    Node *parent;
    size_t start;
    size_t end;
    SiblingIndex child_side; 
} BuildTask;

typedef struct {
    Node *node;
    SiblingIndex branch_side; 
} TreeStackElement;

typedef struct {
    void **node_array;
    size_t current_index;
} FlattenContext;

typedef struct {
    Node *root;
    CompareFunc compare;
    DestroyFunc destroy_key;
    DestroyFunc destroy_value;
    size_t size;
    size_t max_size;
} ScapegoatTree;

typedef void (*TraverseActionFunc)(Node *const node, void *context);

ScapegoatTree *TreeCreate(const CompareFunc cmp, const DestroyFunc destroy_key_func, const DestroyFunc destroy_value_func);
TreeStatus TreeInsert(ScapegoatTree *const tree, void *const key, void *const value);
Vector *TreeFlatten(Node *const root, const size_t sub_tree_size);
Node *TreeRebuild(Node *const old_sub_root, const size_t sub_tree_size);

void *TreeFind(const ScapegoatTree *const tree, const void *const key);
size_t TreeTraverse(Node *const root, const TraverseActionFunc action, void *context);

TreeStatus TreeRemove(ScapegoatTree *const tree, const void *const key);
void TreeFree(ScapegoatTree *tree);

#endif
