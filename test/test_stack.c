#include "../include/stack.h"
#include "../include/util.h"

#include <stdio.h>

int main(void)
{
    Stack *stack = NULL;
    AdtValue value = NULL;
    int a = 10;
    int b = 20;
    AdtStatus status = ADT_OK;

    puts("[DEBUG] start test_stack");
    status = stack_create(&stack);
    debug_status("stack_create", status);
    if (status != ADT_OK || !stack)
    {
        fprintf(stderr, "stack_create failed: %d\n", status);
        return 1;
    }

    printf("[DEBUG] initial size=%zu empty=%d\n", stack_size(stack), (int)stack_is_empty(stack));
    if (!stack_is_empty(stack) || stack_size(stack) != 0)
    {
        fprintf(stderr, "initial stack state invalid\n");
        stack_destroy(stack);
        return 1;
    }

    status = stack_push(stack, &a);
    debug_status("stack_push(a)", status);
    if (status != ADT_OK)
    {
        fprintf(stderr, "stack_push(a) failed: %d\n", status);
        stack_destroy(stack);
        return 1;
    }
    printf("[DEBUG] after push(a) size=%zu\n", stack_size(stack));

    status = stack_push(stack, &b);
    debug_status("stack_push(b)", status);
    if (status != ADT_OK)
    {
        fprintf(stderr, "stack_push(b) failed: %d\n", status);
        stack_destroy(stack);
        return 1;
    }
    printf("[DEBUG] after push(b) size=%zu\n", stack_size(stack));

    if (stack_size(stack) != 2)
    {
        fprintf(stderr, "stack_size after push invalid\n");
        stack_destroy(stack);
        return 1;
    }

    status = stack_peek(stack, &value);
    debug_status("stack_peek", status);
    if (status == ADT_OK)
    {
        printf("[DEBUG] peek value=%d\n", *(int *)value);
    }
    if (status != ADT_OK || value != &b)
    {
        fprintf(stderr, "stack_peek failed: %d\n", status);
        stack_destroy(stack);
        return 1;
    }

    status = stack_pop(stack, &value);
    debug_status("stack_pop #1", status);
    if (status == ADT_OK)
    {
        printf("[DEBUG] pop #1 value=%d size=%zu\n", *(int *)value, stack_size(stack));
    }
    if (status != ADT_OK || value != &b)
    {
        fprintf(stderr, "stack_pop #1 failed: %d\n", status);
        stack_destroy(stack);
        return 1;
    }

    status = stack_pop(stack, &value);
    debug_status("stack_pop #2", status);
    if (status == ADT_OK)
    {
        printf("[DEBUG] pop #2 value=%d size=%zu\n", *(int *)value, stack_size(stack));
    }
    if (status != ADT_OK || value != &a)
    {
        fprintf(stderr, "stack_pop #2 failed: %d\n", status);
        stack_destroy(stack);
        return 1;
    }

    status = stack_pop(stack, &value);
    debug_status("stack_pop #3 (expected empty)", status);
    if (status != ADT_ERR_EMPTY)
    {
        fprintf(stderr, "expected ADT_ERR_EMPTY on empty pop, got: %d\n", status);
        stack_destroy(stack);
        return 1;
    }

    printf("[DEBUG] final size=%zu empty=%d\n", stack_size(stack), (int)stack_is_empty(stack));
    stack_destroy(stack);
    puts("test_stack OK");
    return 0;
}
