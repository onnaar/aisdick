#ifndef NODE_H
#define NODE_H

#include <stdio.h>

typedef enum {
    LEFT = 0,
    RIGHT,
    PARENT
} RelativeIndex; 

typedef struct NodeArray{
    struct Node **node_array;
    size_t size;
} NodeArray;

typedef struct Node {
    struct Node *relatives[3];
    size_t key;
    size_t *info;
} Node;

Node *NodeCreate(Node *parent, size_t key, size_t *info);
Node *NodeCopy(const Node *const node);
NodeArray *NodeArrayManage(NodeArray *array, size_t count);
void NodeDelete(Node *node);

#endif
