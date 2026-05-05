#include <stdlib.h>
#include "nodeArray.h"

SpSearchStructure *SpSearchStructureCreate() {
    return (SpSearchStructure *)calloc(1, sizeof(SpSearchStructure));
}

NodeArray *NodeArrayCreate() {
    NodeArray *array = (NodeArray *)calloc(1, sizeof(NodeArray));
    if (!array) {
        return NULL;
    }
    array->capacity = 1;
    array->node_array = (Node **)calloc(1, sizeof(Node *));
    if (!array->node_array) {
        return NULL;
    }
    return array;
}

NodeArray *NodeArraySizeAppend(NodeArray *const array) {
    if (!array || !array->node_array) {
        return NULL;
    }
    array->capacity *= 2;
    Node **new_ar = (Node **)realloc(array->node_array, array->capacity * sizeof(Node *));
    if (!new_ar) {
        return NULL;
    }
    array->node_array = new_ar;
    return array;
}

void NodeArrayAdd(NodeArray *const array, Node *node) {
    if (!array || !node || !array->node_array) {
        return;
    }
    if (array->size == array->capacity) {
        NodeArray *new = NodeArraySizeAppend(array);
        if (!new) {
            return;
        }
    }
    array->node_array[array->size] = node;
    array->size++;
}

void NodeArrayDelete(NodeArray *array) {
    if (!array) {
        return;
    }
    if (array->node_array) {
        free(array->node_array);
    }
    free(array);
}

void SpSearchStructureDelete(SpSearchStructure *data) {
    if (!data) {
        return;
    }
    if (data->array) {
        if (data->array->node_array) {
            free(data->array->node_array);
        }
        free(data->array);
    }
    free(data);
}

