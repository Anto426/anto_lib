#ifndef QUEUE_H
#define QUEUE_H

#include "common.h"

typedef struct Queue Queue;

AdtStatus queue_create(Queue **out_queue);
void queue_destroy(Queue *queue);

AdtStatus queue_enqueue(Queue *queue, AdtValue value);
AdtStatus queue_dequeue(Queue *queue, AdtValue *out_value);
AdtStatus queue_peek(const Queue *queue, AdtValue *out_value);

bool queue_is_empty(const Queue *queue);
size_t queue_size(const Queue *queue);

#endif // QUEUE_H
