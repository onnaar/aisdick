#include <stdlib.h>
#include "map.h"
#include "scapegoat.h"

struct BoardMap {
    ScapegoatTree *tree;
};

static int ComparePoints(const void *const key1, const void *const key2) {
    Point *p1 = (Point *)key1;
    Point *p2 = (Point *)key2;
    if (p1->x != p2->x) {
        return (p1->x > p2->x) ? 1 : -1;
    }
    if (p1->y != p2->y) {
        return (p1->y > p2->y) ? 1 : -1;
    }
    return 0;
}

BoardMap *MapCreate(void) {
    BoardMap *map = calloc(1, sizeof(BoardMap));
    if (!map) {
        return NULL;
    }
    map->tree = TreeCreate(ComparePoints, free, free);
    if (!map->tree) {
        free(map);
        return NULL;
    }
    return map;
}

MapStatus MapInsert(BoardMap *const map, const Point point, const CellType type) {
    if (!map || !map->tree) {
        return MAP_NOT_VALID;
    }
    Point *key = calloc(1, sizeof(Point));
    if (!key) {
        return MAP_MEMORY_ERROR;
    }
    CellType *val = calloc(1, sizeof(CellType));
    if (!val) {
        free(key);
        return MAP_MEMORY_ERROR;
    }
    key->x = point.x;
    key->y = point.y;
    *val = type;
    TreeStatus stat = TreeInsert(map->tree, key, val);
    if (stat != TREE_OK) {
        free(key);
        free(val);
        return MAP_MEMORY_ERROR;
    }
    return MAP_OK;
}

MapStatus MapGet(BoardMap *const map, const Point point, CellType *const out_type) {
    if (!map || !map->tree || !out_type) {
        return MAP_NOT_VALID;
    }
    CellType *val = (CellType *)TreeFind(map->tree, &point);
    if (!val) {
        return MAP_NOT_FOUND;
    }
    *out_type = *val;
    return MAP_OK;
}

MapStatus MapRemove(BoardMap *const map, const Point point) {
    if (!map || !map->tree) {
        return MAP_NOT_VALID;
    }
    TreeStatus stat = TreeRemove(map->tree, &point);
    if (stat == TREE_NOT_FOUND) {
        return MAP_NOT_FOUND;
    }
    return MAP_OK;
}

void MapFree(BoardMap *const map) {
    if (!map) {
        return;
    }
    if (map->tree) {
        TreeFree(map->tree);
    }
    free(map);
}
