#include <stdlib.h>
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

void NodeDelete(Node *node) {
    free(node->info);
    free(node);
}
