#ifndef STACK_H
#define STACK_H

#include "common.h"

typedef struct Stack Stack;

AdtStatus stack_create(Stack **out_stack);
void stack_destroy(Stack *stack);

AdtStatus stack_push(Stack *stack, AdtValue value);
AdtStatus stack_pop(Stack *stack, AdtValue *out_value);
AdtStatus stack_peek(const Stack *stack, AdtValue *out_value);

bool stack_is_empty(const Stack *stack);
size_t stack_size(const Stack *stack);

#endif // STACK_H
