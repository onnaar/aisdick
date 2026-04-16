#include <stdio.h>
#include <stdlib.h>
#include "node.h"

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

Node *NodeCopy(const Node *const node) {
    if (!node) {
        return NULL;
    }
    Node *new = (Node *)calloc(1, sizeof(Node));
    if (!new) {
        return NULL;
    }
    size_t value = *node->info;
    new->info = &value; 
    new->key = node->key;
    new->relatives[PARENT] = node->relatives[PARENT];
    new->relatives[LEFT] = node->relatives[LEFT];
    new->relatives[RIGHT] = node->relatives[RIGHT];
    return new;
}

void NodeDelete(Node *node) {
    if (!node) {
        return;
    } 
    free(node->info);
    free(node);
}
