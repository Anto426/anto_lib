#include "../include/list.h"

#include <stdlib.h>

typedef struct ListNode
{
    AdtValue value;
    struct ListNode *next;
} ListNode;

struct List
{
    ListNode *head;
    ListNode *tail;
    size_t size;
};

static ListNode *list_node_create(AdtValue value)
{
    ListNode *node = (ListNode *)malloc(sizeof(ListNode));
    if (!node)
    {
        return NULL;
    }

    node->value = value;
    node->next = NULL;
    return node;
}

AdtStatus list_create(List **out_list)
{
    List *list = NULL;

    if (!out_list)
    {
        return ADT_ERR_NULL;
    }

    list = (List *)malloc(sizeof(List));
    if (!list)
    {
        *out_list = NULL;
        return ADT_ERR_ALLOC;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    *out_list = list;
    return ADT_OK;
}

void list_destroy(List *list)
{
    ListNode *curr = NULL;

    if (!list)
    {
        return;
    }

    curr = list->head;
    while (curr)
    {
        ListNode *next = curr->next;
        free(curr);
        curr = next;
    }

    free(list);
}

AdtStatus list_push_front(List *list, AdtValue value)
{
    ListNode *node = NULL;

    if (!list)
    {
        return ADT_ERR_NULL;
    }

    if (list->size == SIZE_MAX)
    {
        return ADT_ERR_OVERFLOW;
    }

    node = list_node_create(value);
    if (!node)
    {
        return ADT_ERR_ALLOC;
    }

    node->next = list->head;
    list->head = node;
    if (list->size == 0)
    {
        list->tail = node;
    }

    list->size++;
    return ADT_OK;
}

AdtStatus list_push_back(List *list, AdtValue value)
{
    ListNode *node = NULL;

    if (!list)
    {
        return ADT_ERR_NULL;
    }

    if (list->size == SIZE_MAX)
    {
        return ADT_ERR_OVERFLOW;
    }

    node = list_node_create(value);
    if (!node)
    {
        return ADT_ERR_ALLOC;
    }

    if (list->tail)
    {
        list->tail->next = node;
    }
    else
    {
        list->head = node;
    }

    list->tail = node;
    list->size++;
    return ADT_OK;
}

AdtStatus list_pop_front(List *list, AdtValue *out_value)
{
    ListNode *node = NULL;

    if (!list || !out_value)
    {
        return ADT_ERR_NULL;
    }

    if (list->size == 0 || !list->head)
    {
        return ADT_ERR_EMPTY;
    }

    node = list->head;
    *out_value = node->value;
    list->head = node->next;
    if (!list->head)
    {
        list->tail = NULL;
    }

    free(node);
    list->size--;
    return ADT_OK;
}

AdtStatus list_peek_front(const List *list, AdtValue *out_value)
{
    if (!list || !out_value)
    {
        return ADT_ERR_NULL;
    }

    if (list->size == 0 || !list->head)
    {
        return ADT_ERR_EMPTY;
    }

    *out_value = list->head->value;
    return ADT_OK;
}

AdtStatus list_peek_back(const List *list, AdtValue *out_value)
{
    if (!list || !out_value)
    {
        return ADT_ERR_NULL;
    }

    if (list->size == 0 || !list->tail)
    {
        return ADT_ERR_EMPTY;
    }

    *out_value = list->tail->value;
    return ADT_OK;
}

AdtStatus list_get_at(const List *list, size_t index, AdtValue *out_value)
{
    size_t i = 0;
    ListNode *curr = NULL;

    if (!list || !out_value)
    {
        return ADT_ERR_NULL;
    }

    if (index >= list->size)
    {
        return ADT_ERR_OUT_OF_RANGE;
    }

    curr = list->head;
    while (curr && i < index)
    {
        curr = curr->next;
        i++;
    }

    if (!curr)
    {
        return ADT_ERR_CORRUPTED;
    }

    *out_value = curr->value;
    return ADT_OK;
}

bool list_is_empty(const List *list)
{
    if (!list)
    {
        return true;
    }

    return (list->size == 0);
}

size_t list_size(const List *list)
{
    if (!list)
    {
        return 0;
    }

    return list->size;
}
