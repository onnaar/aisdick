#include "node.h"
#include <stdlib.h>

Node *NodeCreate(Node *parent, size_t key, size_t *info) {
    if (!parent || !info) {
        return NULL;
    }
    Node *node = (Node *)calloc(1, sizeof(Node));
    if (!node) {
        return NULL;
    }
    node->relatives[PARENT] = parent;
    node->key = key;
    node->info = info;
    return node;
}

void NodeDelete(Node *node) {
    if (!node) {
        return;
    } 
    free(node->info);
    free(node);
}
