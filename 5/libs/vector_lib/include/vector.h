#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#define VECTOR_INITIAL_CAPACITY 4

typedef enum {
    VECTOR_OK,
    VECTOR_NOT_VALID,
    VECTOR_NOT_FOUND,
    VECTOR_MEMORY_ERROR
} VectorStatus;

typedef struct {
    void **data;
    size_t size;
    size_t capacity;
} Vector;

Vector *VectorCreate(size_t initial_capacity);

VectorStatus VectorPush(Vector *vector, void *item);
void *VectorPop(Vector *vector);

VectorStatus VectorRemoveSwap(Vector *vector, size_t index, void (*free_item)(void *));
void VectorFree(Vector *vector, void (*free_item)(void *));
#endif
