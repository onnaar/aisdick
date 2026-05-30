#include <stdlib.h>
#include "vector.h"

Vector *VectorCreate(size_t initial_capacity) {
    Vector *vector = (Vector *)calloc(1, sizeof(Vector));
    if (!vector) {
        return NULL;
    }
    vector->size = 0;
    vector->capacity = initial_capacity;
    vector->data = (void **)calloc(vector->capacity, sizeof(void *));
    if (!vector->data) {
        free(vector);
        return NULL;
    }
    return vector;
}

VectorStatus VectorPush(Vector *vector, void *item) {
    if (!vector || !vector->data) {
        return VECTOR_NOT_VALID;
    }
    if (vector->size >= vector->capacity) {
        size_t new_capacity = vector->capacity * 2;
        void **new_data = (void **)calloc(new_capacity, sizeof(void *));
        if (!new_data) {
            return VECTOR_MEMORY_ERROR;
        }
        for (size_t i = 0; i < vector->size; i++) {
            new_data[i] = vector->data[i];
        }
        free(vector->data);
        vector->data = new_data;
        vector->capacity = new_capacity;
    }
    vector->data[vector->size] = item;
    vector->size++;
    return VECTOR_OK;
}

void *VectorPop(Vector *vector) {
    if (!vector || vector->size == 0) {
        return NULL;
    }
    vector->size--;
    void *item = vector->data[vector->size];
    vector->data[vector->size] = NULL;
    return item;
}

VectorStatus VectorRemoveSwap(Vector *vector, size_t index, void (*free_item)(void *)) {
    if (!vector || !vector->data) {
        return VECTOR_NOT_VALID;
    }
    if (index >= vector->size) {
        return VECTOR_NOT_FOUND;
    }
    if (free_item && vector->data[index]) {
        free_item(vector->data[index]);
    }
    size_t last_index = vector->size - 1;
    if (index != last_index) {
        vector->data[index] = vector->data[last_index];
    }
    vector->data[last_index] = NULL;
    vector->size--;
    if (vector->size > 0 && vector->size <= vector->capacity / 4 && vector->capacity > VECTOR_INITIAL_CAPACITY) {
        size_t new_capacity = vector->capacity / 2;
        void **new_data = (void **)calloc(new_capacity, sizeof(void *));
        if (new_data) {
            for (size_t i = 0; i < vector->size; i++) {
                new_data[i] = vector->data[i];
            }
            free(vector->data);
            vector->data = new_data;
            vector->capacity = new_capacity;
        }
    }
    return VECTOR_OK;
}

void VectorFree(Vector *vector, void (*free_item)(void *)) {
    if (!vector) {
        return;
    }
    if (free_item && vector->data) {
        for (size_t i = 0; i < vector->size; i++) {
            if (vector->data[i]) {
                free_item(vector->data[i]);
            }
        }
    }
    free(vector->data);
    free(vector);
}
