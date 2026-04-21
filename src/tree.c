#include "../include/tree.h"

#include <stdlib.h>

typedef struct TreeNode
{
    AdtValue value;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

struct Tree
{
    TreeNode *root;
    size_t size;
    TreeCompareFn compare_fn;
};

static int tree_compare(const Tree *tree, AdtValue lhs, AdtValue rhs)
{
    if (tree->compare_fn)
    {
        return tree->compare_fn(lhs, rhs);
    }

    if ((uintptr_t)lhs < (uintptr_t)rhs)
    {
        return -1;
    }
    if ((uintptr_t)lhs > (uintptr_t)rhs)
    {
        return 1;
    }
    return 0;
}

static TreeNode *tree_node_create(AdtValue value)
{
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    if (!node)
    {
        return NULL;
    }

    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static void tree_destroy_nodes(TreeNode *node)
{
    if (!node)
    {
        return;
    }

    tree_destroy_nodes(node->left);
    tree_destroy_nodes(node->right);
    free(node);
}

AdtStatus tree_create(Tree **out_tree, TreeCompareFn compare_fn)
{
    Tree *tree = NULL;

    if (!out_tree)
    {
        return ADT_ERR_NULL;
    }

    tree = (Tree *)malloc(sizeof(Tree));
    if (!tree)
    {
        *out_tree = NULL;
        return ADT_ERR_ALLOC;
    }

    tree->root = NULL;
    tree->size = 0;
    tree->compare_fn = compare_fn;
    *out_tree = tree;
    return ADT_OK;
}

void tree_destroy(Tree *tree)
{
    if (!tree)
    {
        return;
    }

    tree_destroy_nodes(tree->root);
    free(tree);
}

AdtStatus tree_insert(Tree *tree, AdtValue value)
{
    TreeNode *curr = NULL;

    if (!tree)
    {
        return ADT_ERR_NULL;
    }

    if (tree->size == SIZE_MAX)
    {
        return ADT_ERR_OVERFLOW;
    }

    if (!tree->root)
    {
        tree->root = tree_node_create(value);
        if (!tree->root)
        {
            return ADT_ERR_ALLOC;
        }
        tree->size++;
        return ADT_OK;
    }

    curr = tree->root;
    while (curr)
    {
        int cmp = tree_compare(tree, value, curr->value);
        if (cmp == 0)
        {
            return ADT_ERR_DUPLICATE;
        }

        if (cmp < 0)
        {
            if (!curr->left)
            {
                curr->left = tree_node_create(value);
                if (!curr->left)
                {
                    return ADT_ERR_ALLOC;
                }
                tree->size++;
                return ADT_OK;
            }
            curr = curr->left;
        }
        else
        {
            if (!curr->right)
            {
                curr->right = tree_node_create(value);
                if (!curr->right)
                {
                    return ADT_ERR_ALLOC;
                }
                tree->size++;
                return ADT_OK;
            }
            curr = curr->right;
        }
    }

    return ADT_ERR_UNKNOWN;
}

AdtStatus tree_remove(Tree *tree, AdtValue value)
{
    TreeNode *parent = NULL;
    TreeNode *curr = NULL;

    if (!tree)
    {
        return ADT_ERR_NULL;
    }

    if (!tree->root || tree->size == 0)
    {
        return ADT_ERR_NOT_FOUND;
    }

    curr = tree->root;
    while (curr)
    {
        int cmp = tree_compare(tree, value, curr->value);
        if (cmp == 0)
        {
            break;
        }

        parent = curr;
        if (cmp < 0)
        {
            curr = curr->left;
        }
        else
        {
            curr = curr->right;
        }
    }

    if (!curr)
    {
        return ADT_ERR_NOT_FOUND;
    }

    if (curr->left && curr->right)
    {
        TreeNode *succ_parent = curr;
        TreeNode *succ = curr->right;

        while (succ->left)
        {
            succ_parent = succ;
            succ = succ->left;
        }

        curr->value = succ->value;
        parent = succ_parent;
        curr = succ;
    }

    {
        TreeNode *child = curr->left ? curr->left : curr->right;

        if (!parent)
        {
            tree->root = child;
        }
        else if (parent->left == curr)
        {
            parent->left = child;
        }
        else
        {
            parent->right = child;
        }
    }

    free(curr);
    tree->size--;
    return ADT_OK;
}

AdtStatus tree_contains(const Tree *tree, AdtValue value, bool *out_contains)
{
    TreeNode *curr = NULL;

    if (!tree || !out_contains)
    {
        return ADT_ERR_NULL;
    }

    curr = tree->root;
    while (curr)
    {
        int cmp = tree_compare(tree, value, curr->value);
        if (cmp == 0)
        {
            *out_contains = true;
            return ADT_OK;
        }

        curr = (cmp < 0) ? curr->left : curr->right;
    }

    *out_contains = false;
    return ADT_OK;
}

AdtStatus tree_find_min(const Tree *tree, AdtValue *out_value)
{
    TreeNode *curr = NULL;

    if (!tree || !out_value)
    {
        return ADT_ERR_NULL;
    }

    if (!tree->root || tree->size == 0)
    {
        return ADT_ERR_EMPTY;
    }

    curr = tree->root;
    while (curr->left)
    {
        curr = curr->left;
    }

    *out_value = curr->value;
    return ADT_OK;
}

bool tree_is_empty(const Tree *tree)
{
    if (!tree)
    {
        return true;
    }

    return (tree->size == 0);
}

size_t tree_size(const Tree *tree)
{
    if (!tree)
    {
        return 0;
    }

    return tree->size;
}
