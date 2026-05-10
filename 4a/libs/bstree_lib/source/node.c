#include <stdlib.h>
#include <string.h>
#include "node_info.h"
#include "input.h"
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
        free(node);
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
    return (node->relatives[LEFT] != NULL) + (node->relatives[RIGHT] != NULL);
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
    NodeInfoDelete(node->info); 
    free(node);
}

char *NodeInfoToString(const Node *const node) {
    if (!node || !node->info) {
        return NULL;
    }
    char *res = NULL;
    if (ZuToStr(node->info->info, &res) != INPUT_OK) {
        return NULL;
    }
    return res;
}

char *NodeKeyToString(const Node *const node) {
    if (!node) {
        return NULL;
    }
    char *key_str = NULL;
    if (ZuToStr(node->key, &key_str) != INPUT_OK) {
        return NULL;
    }
    return key_str;
}

char *NodeToString(const Node *const node) {
    if (!node) {
        return NULL;
    }
    char *key_str = NULL;
    if (ZuToStr(node->key, &key_str) != INPUT_OK) {
        return NULL;
    }
    char *info_str = NodeInfoToString(node);
    if (!info_str) {
        free(key_str); 
        return NULL;
    }
    const char *sep = " - ";
    size_t total_len = strlen(key_str) + strlen(sep) + strlen(info_str);
    char *result = (char *)calloc(total_len + 1, sizeof(char));
    if (result) {
        strcpy(result, key_str);
        strcat(result, sep);
        strcat(result, info_str);
    }
    free(key_str);
    free(info_str);
    return result;
}
