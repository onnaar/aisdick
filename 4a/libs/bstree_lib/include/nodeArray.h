#ifndef NODE_ARRAY_H
#define NODE_ARRAY_H

#include "node.h"

typedef struct NodeArray {
    struct Node **node_array;
    size_t size;
    size_t capacity;
} NodeArray;

typedef struct {
    NodeArray *array;
    size_t key; 
    size_t max_delta;
} SpSearchStructure;

NodeArray *NodeArrayCreate();
SpSearchStructure *SpSearchStructureCreate();

NodeArray *NodeArraySizeAppend(NodeArray *const array);
void NodeArrayAdd(NodeArray *array, Node *const node);

void NodeArrayDelete(NodeArray *array);
void SpSearchStructureDelete(SpSearchStructure *data);

#endif
