#ifndef LIST_H
#define LIST_H

#include "common.h"

typedef struct List List;

AdtStatus list_create(List **out_list);
void list_destroy(List *list);

AdtStatus list_push_front(List *list, AdtValue value);
AdtStatus list_push_back(List *list, AdtValue value);
AdtStatus list_pop_front(List *list, AdtValue *out_value);

AdtStatus list_peek_front(const List *list, AdtValue *out_value);
AdtStatus list_peek_back(const List *list, AdtValue *out_value);
AdtStatus list_get_at(const List *list, size_t index, AdtValue *out_value);

bool list_is_empty(const List *list);
size_t list_size(const List *list);

#endif // LIST_H
