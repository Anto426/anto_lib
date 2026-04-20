#include "../include/stack.h"

#include <stdlib.h>

typedef struct StackNode
{
    AdtValue value;
    struct StackNode *next;
} StackNode;

struct Stack
{
    StackNode *top;
    size_t size;
};

AdtStatus stack_create(Stack **out_stack)
{
    Stack *stack = NULL;

    if (!out_stack)
    {
        return ADT_ERR_NULL;
    }

    stack = (Stack *)malloc(sizeof(Stack));
    if (!stack)
    {
        *out_stack = NULL;
        return ADT_ERR_ALLOC;
    }

    stack->top = NULL;
    stack->size = 0;
    *out_stack = stack;

    return ADT_OK;
}

void stack_destroy(Stack *stack)
{
    StackNode *curr = NULL;

    if (!stack)
    {
        return;
    }

    curr = stack->top;
    while (curr)
    {
        StackNode *next = curr->next;
        free(curr);
        curr = next;
    }

    free(stack);
}

AdtStatus stack_push(Stack *stack, AdtValue value)
{
    StackNode *node = NULL;

    if (!stack)
    {
        return ADT_ERR_NULL;
    }

    if (stack->size == SIZE_MAX)
    {
        return ADT_ERR_OVERFLOW;
    }

    node = (StackNode *)malloc(sizeof(StackNode));
    if (!node)
    {
        return ADT_ERR_ALLOC;
    }

    node->value = value;
    node->next = stack->top;
    stack->top = node;
    stack->size++;

    return ADT_OK;
}

AdtStatus stack_pop(Stack *stack, AdtValue *out_value)
{
    StackNode *node = NULL;

    if (!stack || !out_value)
    {
        return ADT_ERR_NULL;
    }

    if (stack->size == 0 || !stack->top)
    {
        return ADT_ERR_EMPTY;
    }

    node = stack->top;
    *out_value = node->value;
    stack->top = node->next;
    stack->size--;
    free(node);

    return ADT_OK;
}

AdtStatus stack_peek(const Stack *stack, AdtValue *out_value)
{
    if (!stack || !out_value)
    {
        return ADT_ERR_NULL;
    }

    if (stack->size == 0 || !stack->top)
    {
        return ADT_ERR_EMPTY;
    }

    *out_value = stack->top->value;
    return ADT_OK;
}

bool stack_is_empty(const Stack *stack)
{
    if (!stack)
    {
        return true;
    }

    return (stack->size == 0);
}

size_t stack_size(const Stack *stack)
{
    if (!stack)
    {
        return 0;
    }

    return stack->size;
}
