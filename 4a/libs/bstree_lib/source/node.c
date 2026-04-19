#include <stdlib.h>
#include "node.h"

Node *NodeCreate(Node *parent, size_t key, size_t *info) {
    if (!info) {
        return NULL;
    }
    Node *node = (Node *)calloc(1, sizeof(Node));
    if (!node) {
        return NULL;
    }
    node->relatives[PARENT] = parent;
    node->key = key;
    size_t *new_info = (size_t *)calloc(1, sizeof(size_t));
    if (!new_info) {
        return NULL;
    };
    *new_info = *info;
    node->info = new_info;
    return node;
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

void NodeDelete(Node *node) {
    if (!node) {
        return;
    } 
    NodeDelete(node->relatives[LEFT]);
    NodeDelete(node->relatives[RIGHT]);
    free(node->info);
    free(node);
}
