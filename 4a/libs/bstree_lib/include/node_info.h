#ifndef NODE_INFO_H
#define NODE_INFO_H

#include <stdio.h>

typedef struct NodeInfo {
    size_t info;
} NodeInfo;

NodeInfo *NodeInfoCreate(void);
void NodeInfoDelete(NodeInfo *info);

#endif
