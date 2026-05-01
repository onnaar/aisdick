#ifndef NODE_H
#define NODE_H

#include <stdio.h>

typedef enum {
    LEFT = 0,
    RIGHT = 1,
    PARENT = 2
} RelativeIndex; 

typedef struct NodeInfo {
    size_t info;
} NodeInfo;

typedef struct NodeArray {
    struct Node **node_array;
    size_t size;
    size_t capacity;
} NodeArray;

typedef struct Node {
    struct Node *relatives[3];
    size_t key;
    NodeInfo *info;
} Node;

Node *NodeCreate(Node *parent, size_t key, NodeInfo *info);
NodeArray *NodeArrayCreate();

NodeArray *NodeArraySizeAppend(NodeArray *array);
void NodeArrayAdd(NodeArray *array, Node *node);
Node *NodeCopy(const Node *const node);

void NodeArrayDelete(NodeArray *array);
void NodeDelete(Node *node);

#endif
