#include "../include/queue.h"

#include <stdlib.h>

typedef struct QueueNode
{
    AdtValue value;
    struct QueueNode *next;
} QueueNode;

struct Queue
{
    QueueNode *front;
    QueueNode *back;
    size_t size;
};

static QueueNode *queue_node_create(AdtValue value)
{
    QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));
    if (!node)
    {
        return NULL;
    }

    node->value = value;
    node->next = NULL;
    return node;
}

AdtStatus queue_create(Queue **out_queue)
{
    Queue *queue = NULL;

    if (!out_queue)
    {
        return ADT_ERR_NULL;
    }

    queue = (Queue *)malloc(sizeof(Queue));
    if (!queue)
    {
        *out_queue = NULL;
        return ADT_ERR_ALLOC;
    }

    queue->front = NULL;
    queue->back = NULL;
    queue->size = 0;
    *out_queue = queue;
    return ADT_OK;
}

void queue_destroy(Queue *queue)
{
    QueueNode *curr = NULL;

    if (!queue)
    {
        return;
    }

    curr = queue->front;
    while (curr)
    {
        QueueNode *next = curr->next;
        free(curr);
        curr = next;
    }

    free(queue);
}

AdtStatus queue_enqueue(Queue *queue, AdtValue value)
{
    QueueNode *node = NULL;

    if (!queue)
    {
        return ADT_ERR_NULL;
    }

    if (queue->size == SIZE_MAX)
    {
        return ADT_ERR_OVERFLOW;
    }

    node = queue_node_create(value);
    if (!node)
    {
        return ADT_ERR_ALLOC;
    }

    if (queue->back)
    {
        queue->back->next = node;
    }
    else
    {
        queue->front = node;
    }

    queue->back = node;
    queue->size++;
    return ADT_OK;
}

AdtStatus queue_dequeue(Queue *queue, AdtValue *out_value)
{
    QueueNode *node = NULL;

    if (!queue || !out_value)
    {
        return ADT_ERR_NULL;
    }

    if (queue->size == 0 || !queue->front)
    {
        return ADT_ERR_EMPTY;
    }

    node = queue->front;
    *out_value = node->value;
    queue->front = node->next;
    if (!queue->front)
    {
        queue->back = NULL;
    }

    free(node);
    queue->size--;
    return ADT_OK;
}

AdtStatus queue_peek(const Queue *queue, AdtValue *out_value)
{
    if (!queue || !out_value)
    {
        return ADT_ERR_NULL;
    }

    if (queue->size == 0 || !queue->front)
    {
        return ADT_ERR_EMPTY;
    }

    *out_value = queue->front->value;
    return ADT_OK;
}

bool queue_is_empty(const Queue *queue)
{
    if (!queue)
    {
        return true;
    }

    return (queue->size == 0);
}

size_t queue_size(const Queue *queue)
{
    if (!queue)
    {
        return 0;
    }

    return queue->size;
}
