#include "../include/tree.h"
#include "../include/util.h"

#include <stdio.h>

static int int_compare(AdtValue lhs, AdtValue rhs)
{
    int a = *(int *)lhs;
    int b = *(int *)rhs;

    if (a < b)
    {
        return -1;
    }
    if (a > b)
    {
        return 1;
    }
    return 0;
}

int main(void)
{
    Tree *tree = NULL;
    AdtValue value = NULL;
    bool contains = false;
    AdtStatus status = ADT_OK;

    int n5 = 5;
    int n3 = 3;
    int n7 = 7;
    int n6 = 6;
    int n9 = 9;
    int n99 = 99;

    puts("[DEBUG] start test_tree");
    status = tree_create(&tree, int_compare);
    debug_status("tree_create", status);
    if (status != ADT_OK || !tree)
    {
        fprintf(stderr, "tree_create failed: %d\n", status);
        return 1;
    }

    status = tree_insert(tree, &n5);
    debug_status("tree_insert(5)", status);
    status = tree_insert(tree, &n3);
    debug_status("tree_insert(3)", status);
    status = tree_insert(tree, &n7);
    debug_status("tree_insert(7)", status);
    status = tree_insert(tree, &n6);
    debug_status("tree_insert(6)", status);
    if (tree_size(tree) != 4)
    {
        fprintf(stderr, "tree size invalid after insert\n");
        tree_destroy(tree);
        return 1;
    }

    status = tree_insert(tree, &n7);
    debug_status("tree_insert(7 duplicate)", status);
    if (status != ADT_ERR_DUPLICATE)
    {
        fprintf(stderr, "expected ADT_ERR_DUPLICATE, got: %d\n", status);
        tree_destroy(tree);
        return 1;
    }

    status = tree_find_min(tree, &value);
    debug_status("tree_find_min", status);
    if (status != ADT_OK || value != &n3)
    {
        fprintf(stderr, "tree_find_min invalid\n");
        tree_destroy(tree);
        return 1;
    }

    status = tree_contains(tree, &n3, &contains);
    debug_status("tree_contains(3)", status);
    if (status != ADT_OK || !contains)
    {
        fprintf(stderr, "tree_contains(3) invalid\n");
        tree_destroy(tree);
        return 1;
    }

    status = tree_contains(tree, &n9, &contains);
    debug_status("tree_contains(9)", status);
    if (status != ADT_OK || contains)
    {
        fprintf(stderr, "tree_contains(9) invalid\n");
        tree_destroy(tree);
        return 1;
    }

    status = tree_remove(tree, &n5);
    debug_status("tree_remove(5)", status);
    if (status != ADT_OK || tree_size(tree) != 3)
    {
        fprintf(stderr, "tree_remove(5) invalid\n");
        tree_destroy(tree);
        return 1;
    }

    status = tree_contains(tree, &n5, &contains);
    debug_status("tree_contains(5 after remove)", status);
    if (status != ADT_OK || contains)
    {
        fprintf(stderr, "tree_contains(5) invalid after remove\n");
        tree_destroy(tree);
        return 1;
    }

    status = tree_remove(tree, &n99);
    debug_status("tree_remove(99 expected not found)", status);
    if (status != ADT_ERR_NOT_FOUND)
    {
        fprintf(stderr, "expected ADT_ERR_NOT_FOUND, got: %d\n", status);
        tree_destroy(tree);
        return 1;
    }

    tree_destroy(tree);
    puts("test_tree OK");
    return 0;
}
