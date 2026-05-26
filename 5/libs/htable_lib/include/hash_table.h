#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdbool.h>

#define INIT_CAPACITY 4
#define MAX_LOAD_FACTOR_PERCENT 70

typedef size_t (*HashFunc)(const void *const key, const size_t capacity);
typedef int (*CompareFunc)(const void *const key1, const void *const key2);
typedef void (*DestroyFunc)(void *const data);
typedef bool (*HashAction)(void);

typedef enum {
    DELETED = -1,
    EMPTY = 0,
    BUSY = 1,
} status;

typedef enum {
    TABLE_OK = 0,
    TABLE_NOT_VALID,
    TABLE_NOT_FOUND,
    TABLE_EMPTY,
    TABLE_MEMORY_ERROR,
    TABLE_WRONG_FORMAT,
    TABLE_END_OF_INPUT,
    TABLE_DUPLICATE
} TableStatus;

typedef struct HashEntry {
    void *key;
    void *value;
    status state;
} HashEntry;

typedef struct HashTable {
    HashEntry *buckets;
    HashFunc hash1;
    HashFunc hash2;
    CompareFunc compare;
    DestroyFunc destroy_key;
    DestroyFunc destroy_value;
    size_t capacity;
    size_t size;
    size_t tombstones;
} HashTable;

HashTable *TableCreate(const HashFunc h1, const HashFunc h2, \
        const CompareFunc cmp, const DestroyFunc destroy_key_func, const DestroyFunc destroy_value_func);

TableStatus TableResize(HashTable *const table);
TableStatus TableInsert(HashTable *const table, void *const key, void *const value);
TableStatus TableRemove(HashTable *const table, const void *const key);

void *TableFind(const HashTable *const table, const void *const key);

void TableFree(HashTable *const table);

#endif
