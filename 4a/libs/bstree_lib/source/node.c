#include <stdlib.h>
#include "node.h"

Node *NodeCreate(Node *const parent, const size_t key, const NodeInfo *const info) {
    if (!info) {
        return NULL;
    }
    Node *node = (Node *)calloc(1, sizeof(Node));
    if (!node) {
        return NULL;
    }
    node->relatives[PARENT] = parent;
    node->key = key;
    NodeInfo *new_info = NodeInfoCreate();   
    if (!new_info) {
        return NULL;
    };
    *new_info = *info;
    node->info = new_info;
    return node;
}

size_t ChildrenCounter(const Node *const node) {
    if (!node) {
        return 0;
    }
    size_t children_number = 0;
    if (node->relatives[LEFT]) {
        children_number++;
    }
    if (node->relatives[RIGHT]) {
        children_number++;
    }
    return children_number;
}

Node *NodeCopy(const Node *const node) {
    if (!node) {
        return NULL;
    }
    Node *new = NodeCreate(node->relatives[PARENT], node->key, node->info);
    if (!new) {
        return NULL;
    }
    new->relatives[LEFT] = node->relatives[LEFT];
    new->relatives[RIGHT] = node->relatives[RIGHT];
    return new;
}

NodeInfo *NodeInfoCreate() {
    return (NodeInfo *)calloc(1, sizeof(NodeInfo));
}

void NodeInfoDelete(NodeInfo *info) {
    if (info) {
        free(info);
    }
}

void NodeDelete(Node *node) {
    if (!node) {
        return;
    }
    NodeInfoDelete(node->info); 
    free(node);
}
