#ifndef MAP_H
#define MAP_H

#include <stdio.h>

typedef struct {
    size_t x;
    size_t y;
} Point;

typedef enum {
    CELL_EMPTY = 0,
    CELL_CROSS,
    CELL_NOUGHT
} CellType;

typedef enum {
    MAP_OK = 0,
    MAP_NOT_VALID,
    MAP_NOT_FOUND,
    MAP_EMPTY,
    MAP_MEMORY_ERROR,
    MAP_WRONG_FORMAT,
    MAP_END_OF_INPUT,
    MAP_DUPLICATE
} MapStatus;

typedef struct {
    Point point;
    CellType type;
} Cell;

typedef struct BoardMap BoardMap;

BoardMap* MapCreate();

MapStatus MapInsert(BoardMap* map, Point point, CellType type);
MapStatus MapGet(BoardMap* map, Point point, CellType* out_type);
MapStatus MapRemove(BoardMap* map, Point point);

void MapFree(BoardMap* map);

#endif
