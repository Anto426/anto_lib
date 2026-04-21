#include "../include/list.h"
#include "../include/util.h"

#include <stdio.h>

int main(void)
{
    List *list = NULL;
    AdtValue value = NULL;
    int a = 10;
    int b = 20;
    int c = 30;
    AdtStatus status = ADT_OK;

    puts("[DEBUG] start test_list");
    status = list_create(&list);
    debug_status("list_create", status);
    if (status != ADT_OK || !list)
    {
        fprintf(stderr, "list_create failed: %d\n", status);
        return 1;
    }

    if (!list_is_empty(list) || list_size(list) != 0)
    {
        fprintf(stderr, "initial list state invalid\n");
        list_destroy(list);
        return 1;
    }

    status = list_push_back(list, &a);
    debug_status("list_push_back(a)", status);
    status = list_push_back(list, &b);
    debug_status("list_push_back(b)", status);
    status = list_push_front(list, &c);
    debug_status("list_push_front(c)", status);
    if (list_size(list) != 3)
    {
        fprintf(stderr, "list size invalid after pushes\n");
        list_destroy(list);
        return 1;
    }

    status = list_get_at(list, 0, &value);
    debug_status("list_get_at(0)", status);
    if (status != ADT_OK || value != &c)
    {
        fprintf(stderr, "list_get_at(0) invalid\n");
        list_destroy(list);
        return 1;
    }

    status = list_get_at(list, 1, &value);
    debug_status("list_get_at(1)", status);
    if (status != ADT_OK || value != &a)
    {
        fprintf(stderr, "list_get_at(1) invalid\n");
        list_destroy(list);
        return 1;
    }

    status = list_peek_back(list, &value);
    debug_status("list_peek_back", status);
    if (status != ADT_OK || value != &b)
    {
        fprintf(stderr, "list_peek_back invalid\n");
        list_destroy(list);
        return 1;
    }

    status = list_pop_front(list, &value);
    debug_status("list_pop_front #1", status);
    if (status != ADT_OK || value != &c)
    {
        fprintf(stderr, "list_pop_front #1 invalid\n");
        list_destroy(list);
        return 1;
    }

    status = list_pop_front(list, &value);
    debug_status("list_pop_front #2", status);
    if (status != ADT_OK || value != &a)
    {
        fprintf(stderr, "list_pop_front #2 invalid\n");
        list_destroy(list);
        return 1;
    }

    status = list_pop_front(list, &value);
    debug_status("list_pop_front #3", status);
    if (status != ADT_OK || value != &b)
    {
        fprintf(stderr, "list_pop_front #3 invalid\n");
        list_destroy(list);
        return 1;
    }

    status = list_pop_front(list, &value);
    debug_status("list_pop_front #4 (expected empty)", status);
    if (status != ADT_ERR_EMPTY)
    {
        fprintf(stderr, "expected ADT_ERR_EMPTY, got: %d\n", status);
        list_destroy(list);
        return 1;
    }

    list_destroy(list);
    puts("test_list OK");
    return 0;
}
