#ifndef TREE_H
#define TREE_H

#include "common.h"

typedef struct Tree Tree;
typedef int (*TreeCompareFn)(AdtValue lhs, AdtValue rhs);

AdtStatus tree_create(Tree **out_tree, TreeCompareFn compare_fn);
void tree_destroy(Tree *tree);

AdtStatus tree_insert(Tree *tree, AdtValue value);
AdtStatus tree_remove(Tree *tree, AdtValue value);

AdtStatus tree_contains(const Tree *tree, AdtValue value, bool *out_contains);
AdtStatus tree_find_min(const Tree *tree, AdtValue *out_value);

bool tree_is_empty(const Tree *tree);
size_t tree_size(const Tree *tree);

#endif // TREE_H
