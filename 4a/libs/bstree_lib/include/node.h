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

typedef struct Node {
    struct Node *relatives[3];
    size_t key;
    NodeInfo *info;
} Node;

Node *NodeCreate(Node *const parent, const size_t key, const NodeInfo *const info);
NodeInfo *NodeInfoCreate();

Node *NodeCopy(const Node *const node);

void NodeInfoDelete(NodeInfo *info);
void NodeDelete(Node *node);

#endif
