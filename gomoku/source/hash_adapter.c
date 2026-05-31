#include <stdlib.h>
#include "map.h"
#include "htable.h"

#define FNV_OFFSET 14695981039346656037ULL
#define FNV_PRIME  1099511628211ULL
#define KNUTH_PRIME 11400714819323198485ULL

struct BoardMap {
    HashTable *table;
};

static size_t Hash1(const void *const key, const size_t capacity) {
    Point *point = (Point *)key;
    size_t hash = FNV_OFFSET;
    hash ^= point->x;
    hash *= FNV_PRIME;
    hash ^= point->y;
    hash *= FNV_PRIME;
    return hash % capacity;
}

static size_t Hash2(const void *const key, const size_t capacity) {
    Point *point = (Point *)key;
    size_t hash_x = point->x * KNUTH_PRIME;
    size_t hash_y = point->y * KNUTH_PRIME;
    size_t hash = hash_x ^ hash_y;
    return ((hash % (capacity - 1)) + 1) | 1;
}

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

BoardMap *MapCreate() {
    BoardMap *map = (BoardMap *)calloc(1, sizeof(BoardMap));
    if (!map) {
        return NULL;
    }
    map->table = TableCreate(Hash1, Hash2, ComparePoints, free, free);
    if (!map->table) {
        free(map);
        return NULL;
    }
    return map;
}

MapStatus MapInsert(BoardMap *const map, const Point point, const CellType type) {
    if (!map || !map->table) {
        return MAP_NOT_VALID;
    }
    Point *key = (Point *)calloc(1, sizeof(Point));
    if (!key) {
        return MAP_MEMORY_ERROR;
    }
    CellType *val = (CellType *)calloc(1, sizeof(CellType));
    if (!val) {
        free(key);
        return MAP_MEMORY_ERROR;
    }
    key->x = point.x;
    key->y = point.y;
    *val = type;
    TableStatus stat = TableInsert(map->table, key, val);
    if (stat != TABLE_OK) {
        free(key);
        free(val);
        return MAP_MEMORY_ERROR;
    }
    return MAP_OK;
}

MapStatus MapGet(const BoardMap *const map, const Point point, CellType *const out_type) {
    if (!map || !map->table || !out_type) {
        return MAP_NOT_VALID;
    }
    CellType *val = (CellType *)TableFind(map->table, &point);
    if (!val) {
        return MAP_NOT_FOUND;
    }
    *out_type = *val;
    return MAP_OK;
}

MapStatus MapRemove(BoardMap *const map, const Point point) {
    if (!map || !map->table) {
        return MAP_NOT_VALID;
    }
    TableStatus stat = TableRemove(map->table, &point);
    if (stat == TABLE_NOT_FOUND) {
        return MAP_NOT_FOUND;
    }
    return MAP_OK;
}

void MapFree(BoardMap *const map) {
    if (!map) {
        return;
    }
    if (map->table) {
        TableFree(map->table);
    }
    free(map);
}
