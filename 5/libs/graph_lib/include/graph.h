#ifndef GRAPH_H
#define GRAPH_H

#include "hash_table.h"
#include "vertex.h"
#include "vector.h"
#include <stddef.h>

typedef enum {
    GRAPH_OK = 0,
    GRAPH_NOT_VALID,
    GRAPH_NOT_FOUND,
    GRAPH_EMPTY,
    GRAPH_MEMORY_ERROR,
    GRAPH_WRONG_FORMAT,
    GRAPH_END_OF_INPUT,
    GRAPH_DUPLICATE
} GraphStatus;

typedef struct Graph {
    HashTable *data;
    Vector *id_vector;
} Graph;

typedef struct {
    Vertex *src;
    Vertex *dst;
    Neighbours dir;
} MSTEdge;

typedef enum {
    DFS_UNVISITED = 0,
    DFS_VISITING,
    DFS_GOOD_PATH,
    DFS_DEAD_END
} DFSState;

typedef struct {
    Vertex *vertex;
    Neighbours dir; 
} StackFrame;

size_t GraphHash1(const void *const key, const size_t capacity);
size_t GraphHash2(const void *const key, const size_t capacity);
int GraphComparePoints(const void *const key1, const void *const key2);

Graph *GraphCreate();
GraphStatus GraphAddVertex(Graph *const graph, const Point coords, const VertexType type);
GraphStatus GraphAddEdge(Graph *const graph, const size_t id, const Neighbours direction);

GraphStatus GraphImport(Graph *const graph, const char *const filename);
GraphStatus GraphExport(const Graph *const graph, const char *const filename);

Vertex *GraphFindVertex(const Graph *const graph, const size_t target_id);
GraphStatus GraphUpdateVertex(Graph *const graph, const size_t target_id, const Point new_coords, const VertexType new_type);

GraphStatus GraphMakeMST(Graph *const graph);
Vertex **ShortestPathDijkstra(const Graph *const graph, const size_t start_id, const size_t finish_id);
Vertex **BFS(const Graph *const graph, const size_t start_id);

GraphStatus GraphExportDot(const Graph *const graph, const char *const filename, Vertex **const path);
GraphStatus GraphAdjacencyOutput(const Graph *const graph);

GraphStatus GraphRemoveVertex(Graph *const graph, const size_t id);
GraphStatus GraphRemoveEdge(Graph *const graph, const size_t id, const Neighbours direction);
void GraphFree(Graph *const graph);

#endif
