#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "node.h"

Node *NodeCreate(const InfoType *const info, ReleaseType last_rel) {
    Node *cur = (Node *)calloc(1, sizeof(Node));
    if (!cur) {
        return NULL;
    }
    char *new_info = strdup(info);
    cur->info = new_info;
    cur->release = ++last_rel;
    return cur;
}

Node *NodeCopy(const Node *const node) {
    if (!node) {
        return NULL;
    }
    Node *new_node = NodeCreateRelease(node->info, node->release);
    new_node->next = node->next;
    return new_node;
}

Node *NodeCreateRelease(const InfoType *const info, ReleaseType release) {
    Node *cur = NodeCreate(info, release);
    if (!cur) {
        return NULL;
    }
    cur->release = release; 
    return cur;
}


void NodeDelete(Node *node) {
    free(node->info);
    free(node);
}
