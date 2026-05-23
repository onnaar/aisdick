#include <stdlib.h>
#include "queue.h"

#define INIT_SIZE 4

struct Queue {
    void **data;
    size_t head;
    size_t tail;
    size_t size;
    size_t capacity;
};

Queue *QueueCreate() {
    Queue *queue = (Queue *)calloc(1, sizeof(Queue));
    if (!queue) {
        return NULL;
    }
    queue->data = (void **)calloc(INIT_SIZE, sizeof(void *));
    if (!queue->data) {
        QueueFree(queue);
        return NULL;
    }
    queue->capacity = INIT_SIZE;
    return queue;
}

QueueStatus QueueMemoryAppend(Queue *const queue) {
    if (!queue) {
        return QUEUE_NOT_VALID;
    }

    size_t new_capacity = queue->capacity * 2;
    void **new_data = (void **)malloc(new_capacity * sizeof(void *));
    if (!new_data) {
        return QUEUE_MEMORY_ERROR;
    }
    for (size_t i = 0; i < queue->size; ++i) {
        new_data[i] = queue->data[(queue->head + i) % queue->capacity];
    }
    free(queue->data);
    queue->data = new_data;
    queue->head = 0;
    queue->tail = queue->size; 
    queue->capacity = new_capacity;
    return QUEUE_OK;
}

QueueStatus QueuePush(Queue *const queue, void *const data) {
    if (!queue) {
        return QUEUE_NOT_VALID;
    }
    if (queue->size == queue->capacity) {
        QueueStatus status = QueueMemoryAppend(queue);
        if (status != QUEUE_OK) {
            return status;
        }
    }
    queue->data[queue->tail] = data;
    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->size++;
    return QUEUE_OK;
}

void *QueuePop(Queue *const queue) {
    if (!queue || IsQueueEmpty(queue)) {
        return NULL;
    }
    void *temp = queue->data[queue->head];
    queue->data[queue->head] = NULL;
    queue->head = (queue->head + 1) % queue->capacity;
    queue->size--;
    return temp;
}

bool IsQueueEmpty(const Queue *const queue) {
    return queue->head == queue->tail;
}

void QueueFree(Queue *queue) {
    if (!queue) {
        return;
    }
    free(queue->data);
    free(queue);
}
