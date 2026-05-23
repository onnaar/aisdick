#ifndef VERTEX_H
#define VERTEX_H

#include <stdio.h>

typedef enum VertexType {
    EXIT = -1,
    TRANSITION = 0,
    ENTRANCE = 1,
} VertexType;

typedef enum Neighbours {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3,
} Neighbours;

typedef struct Point {
    size_t x;
    size_t y;
} Point;

typedef struct Vertex {
    struct Vertex *adjacency[4];
    struct Point coords;
    size_t id;
    enum VertexType type; 
} Vertex;

Vertex *VertexCreate();
void VertexFree(void *const value);

#endif
