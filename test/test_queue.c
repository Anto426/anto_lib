#include "../include/queue.h"
#include "../include/util.h"

#include <stdio.h>

int main(void)
{
    Queue *queue = NULL;
    AdtValue value = NULL;
    int a = 10;
    int b = 20;
    int c = 30;
    AdtStatus status = ADT_OK;

    puts("[DEBUG] start test_queue");
    status = queue_create(&queue);
    debug_status("queue_create", status);
    if (status != ADT_OK || !queue)
    {
        fprintf(stderr, "queue_create failed: %d\n", status);
        return 1;
    }

    if (!queue_is_empty(queue) || queue_size(queue) != 0)
    {
        fprintf(stderr, "initial queue state invalid\n");
        queue_destroy(queue);
        return 1;
    }

    status = queue_enqueue(queue, &a);
    debug_status("queue_enqueue(a)", status);
    status = queue_enqueue(queue, &b);
    debug_status("queue_enqueue(b)", status);
    status = queue_enqueue(queue, &c);
    debug_status("queue_enqueue(c)", status);
    if (queue_size(queue) != 3)
    {
        fprintf(stderr, "queue size invalid after enqueue\n");
        queue_destroy(queue);
        return 1;
    }

    status = queue_peek(queue, &value);
    debug_status("queue_peek", status);
    if (status != ADT_OK || value != &a)
    {
        fprintf(stderr, "queue_peek invalid\n");
        queue_destroy(queue);
        return 1;
    }

    status = queue_dequeue(queue, &value);
    debug_status("queue_dequeue #1", status);
    if (status != ADT_OK || value != &a)
    {
        fprintf(stderr, "queue_dequeue #1 invalid\n");
        queue_destroy(queue);
        return 1;
    }

    status = queue_dequeue(queue, &value);
    debug_status("queue_dequeue #2", status);
    if (status != ADT_OK || value != &b)
    {
        fprintf(stderr, "queue_dequeue #2 invalid\n");
        queue_destroy(queue);
        return 1;
    }

    status = queue_dequeue(queue, &value);
    debug_status("queue_dequeue #3", status);
    if (status != ADT_OK || value != &c)
    {
        fprintf(stderr, "queue_dequeue #3 invalid\n");
        queue_destroy(queue);
        return 1;
    }

    status = queue_dequeue(queue, &value);
    debug_status("queue_dequeue #4 (expected empty)", status);
    if (status != ADT_ERR_EMPTY)
    {
        fprintf(stderr, "expected ADT_ERR_EMPTY, got: %d\n", status);
        queue_destroy(queue);
        return 1;
    }

    queue_destroy(queue);
    puts("test_queue OK");
    return 0;
}
