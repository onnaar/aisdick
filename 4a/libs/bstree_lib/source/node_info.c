#include <stdlib.h>
#include "node_info.h"

NodeInfo *NodeInfoCreate(void) {
    return (NodeInfo *)calloc(1, sizeof(NodeInfo));
}

void NodeInfoDelete(NodeInfo *info) {
    if (info) {
        free(info);
    }
}
