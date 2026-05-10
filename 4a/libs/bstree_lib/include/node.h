#ifndef NODE_H
#define NODE_H

#include "node_info.h"

typedef enum {
    LEFT = 0,
    RIGHT = 1,
    PARENT = 2
} RelativeIndex; 

typedef struct Node {
    struct Node *relatives[3];
    size_t key;
    NodeInfo *info;
} Node;

Node *NodeCreate(Node *const parent, const size_t key, const NodeInfo *const info);

size_t ChildrenCounter(const Node *const node);
Node *NodeCopy(const Node *const node);
char *NodeToString(const Node *const node);
char *NodeKeyToString(const Node *const node);
char *NodeInfoToString(const Node *const node);

void NodeDelete(Node *node);

#endif
