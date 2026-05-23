#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

typedef enum  {
    QUEUE_OK = 0,
    QUEUE_NOT_VALID = 1,
    QUEUE_MEMORY_ERROR = 2,
    QUEUE_EMPTY = 3,
} QueueStatus;

typedef struct Queue Queue;

Queue *QueueCreate();

QueueStatus QueuePush(Queue *const queue, void *const data);
void *QueuePop(Queue *const queue);
QueueStatus QueueMemoryAppend(Queue *const queue);
bool IsQueueEmpty(const Queue *const queue);

void QueueFree(Queue *queue);

#endif
