#include <stdlib.h>
#include "htable.h"

HashTable *TableCreate(const HashFunc h1, const HashFunc h2, const CompareFunc cmp, const DestroyFunc destroy_key_func, const DestroyFunc destroy_value_func) {
    HashTable *table = (HashTable *)calloc(1, sizeof(HashTable));
    if (!table) {
        return NULL;
    }
    table->buckets = (HashEntry *)calloc(INIT_CAPACITY, sizeof(HashEntry));
    if (!table->buckets) {
        free(table);
        return NULL;
    }
    table->hash1 = h1;
    table->hash2 = h2;
    table->capacity = INIT_CAPACITY;
    table->compare = cmp;
    table->destroy_key = destroy_key_func;
    table->destroy_value = destroy_value_func;
    return table;
}

TableStatus TableResize(HashTable *table) {
    if (!table) {
        return TABLE_NOT_VALID;
    }
    HashEntry *old_buckets = table->buckets;
    table->buckets = (HashEntry *)calloc(2 * table->capacity, sizeof(HashEntry));
    if (!table->buckets) {
        table->buckets = old_buckets;
        return TABLE_MEMORY_ERROR;
    }
    size_t old_capacity = table->capacity; 
    table->capacity *= 2;
    table->size = 0;
    table->tombstones = 0;
    TableStatus status = TABLE_OK;
    for (size_t i = 0; i < old_capacity; i++) {
        if (old_buckets[i].state == BUSY) {
            status = TableInsert(table, old_buckets[i].key, old_buckets[i].value);
            if (status != TABLE_OK) {
                return status;
            }
        }
    }
    free(old_buckets);
    return TABLE_OK;
}

static inline HashEntry *BucketFind(const HashTable *const table, const void *const key, HashAction use_deleted) {
    size_t h1 = table->hash1(key, table->capacity);
    size_t h2 = table->hash2(key, table->capacity);
    HashEntry *first_deleted = NULL;
    for (size_t i = 0; i < table->capacity; i++) {
        size_t index = (h1 + i * h2) % table->capacity;
        HashEntry *entry = &table->buckets[index];
        if (entry->state == EMPTY) {
            if (use_deleted() && first_deleted) {
                return first_deleted;
            }
            return entry;
        }
        if (entry->state == DELETED && !first_deleted) {
            first_deleted = entry;
        }
        if (entry->state == BUSY && table->compare(entry->key, key) == 0) {
            return entry;
        }
    }
    return (use_deleted() && first_deleted) ? first_deleted : NULL;
}

static inline bool ActionForInsert() {
    return true;
}

static inline bool ActionForFind() {
    return false;
}

TableStatus TableInsert(HashTable *const table, void *const key, void *const value) {
    if (!table || !key || !value) {
        return TABLE_NOT_VALID;
    } 
    size_t load_factor = (table->size + table->tombstones) * 100 / table->capacity;
    TableStatus status = TABLE_OK;
    if (load_factor > MAX_LOAD_FACTOR_PERCENT) {
        status = TableResize(table);
        if (status != TABLE_OK) {
            return status;
        }
    }
    status = TABLE_OK;
    HashEntry *entry = BucketFind(table, key, ActionForInsert);
    if (!entry) {
        return TABLE_MEMORY_ERROR;
    }
    if (entry->state == BUSY) {
        return TABLE_DUPLICATE;
    }
    if (entry->state == DELETED) {
        table->tombstones--;
    }
    entry->key = key;
    entry->value = value;
    entry->state = BUSY;
    table->size++;
    status = TABLE_OK;
    return TABLE_OK;
}

void *TableFind(const HashTable *const table, const void *const key) {
    if (!table || !key) {
        return NULL;
    }
    HashEntry *entry = BucketFind(table, key, ActionForFind);
    return entry ? entry->value : NULL;
}   

TableStatus TableRemove(HashTable *const table, const void *const key) {
    if (!table || !key) {
        return TABLE_NOT_VALID;
    }
    HashEntry *entry = BucketFind(table, key, ActionForFind);
    if (!entry || entry->state != BUSY) {
        return TABLE_NOT_FOUND;
    }
    if (table->destroy_key) {
        table->destroy_key(entry->key);
    }
    if (table->destroy_value) {
        table->destroy_value(entry->value);
    }
    entry->state = DELETED;
    table->size--;
    table->tombstones++;
    return TABLE_OK;
}

void TableFree(HashTable *const table) {
    if (!table) {
        return;
    }
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->buckets[i].state == BUSY) {
            if (table->destroy_key) {
                table->destroy_key(table->buckets[i].key);
            }
            if (table->destroy_value) {
                table->destroy_value(table->buckets[i].value);
            }
        }
    }
    free(table->buckets);
    free(table);
}

