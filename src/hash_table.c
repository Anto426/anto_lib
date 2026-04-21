#include "../include/hash_table.h"

#include <stdlib.h>

typedef struct HashTableNode
{
    AdtValue key;
    AdtValue value;
    struct HashTableNode *next;
} HashTableNode;

struct HashTable
{
    size_t bucket_count;
    size_t size;
    HashTableHashFn hash_fn;
    HashTableEqualsFn equals_fn;
    HashTableNode **buckets;
};

static uint64_t hash_table_default_hash(AdtValue key)
{
    uint64_t x = (uint64_t)(uintptr_t)key;

    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;
    return x;
}

static bool hash_table_default_equals(AdtValue lhs, AdtValue rhs)
{
    return lhs == rhs;
}

static size_t hash_table_index(const HashTable *table, AdtValue key)
{
    HashTableHashFn hash_fn = table->hash_fn ? table->hash_fn : hash_table_default_hash;
    uint64_t hash = hash_fn(key);
    return (size_t)(hash % (uint64_t)table->bucket_count);
}

static bool hash_table_equals(const HashTable *table, AdtValue lhs, AdtValue rhs)
{
    HashTableEqualsFn equals_fn = table->equals_fn ? table->equals_fn : hash_table_default_equals;
    return equals_fn(lhs, rhs);
}

AdtStatus hash_table_create(
    HashTable **out_table,
    size_t bucket_count,
    HashTableHashFn hash_fn,
    HashTableEqualsFn equals_fn)
{
    HashTable *table = NULL;

    if (!out_table)
    {
        return ADT_ERR_NULL;
    }

    if (bucket_count == 0)
    {
        bucket_count = 64;
    }

    if (bucket_count > (SIZE_MAX / sizeof(HashTableNode *)))
    {
        *out_table = NULL;
        return ADT_ERR_OVERFLOW;
    }

    table = (HashTable *)malloc(sizeof(HashTable));
    if (!table)
    {
        *out_table = NULL;
        return ADT_ERR_ALLOC;
    }

    table->buckets = (HashTableNode **)calloc(bucket_count, sizeof(HashTableNode *));
    if (!table->buckets)
    {
        free(table);
        *out_table = NULL;
        return ADT_ERR_ALLOC;
    }

    table->bucket_count = bucket_count;
    table->size = 0;
    table->hash_fn = hash_fn;
    table->equals_fn = equals_fn;
    *out_table = table;
    return ADT_OK;
}

void hash_table_destroy(HashTable *table)
{
    if (!table)
    {
        return;
    }

    hash_table_clear(table);
    free(table->buckets);
    free(table);
}

AdtStatus hash_table_put(HashTable *table, AdtValue key, AdtValue value)
{
    size_t index = 0;
    HashTableNode *curr = NULL;
    HashTableNode *node = NULL;

    if (!table || !table->buckets || table->bucket_count == 0)
    {
        return ADT_ERR_NULL;
    }

    index = hash_table_index(table, key);
    curr = table->buckets[index];
    while (curr)
    {
        if (hash_table_equals(table, curr->key, key))
        {
            curr->value = value;
            return ADT_OK;
        }
        curr = curr->next;
    }

    if (table->size == SIZE_MAX)
    {
        return ADT_ERR_OVERFLOW;
    }

    node = (HashTableNode *)malloc(sizeof(HashTableNode));
    if (!node)
    {
        return ADT_ERR_ALLOC;
    }

    node->key = key;
    node->value = value;
    node->next = table->buckets[index];
    table->buckets[index] = node;
    table->size++;
    return ADT_OK;
}

AdtStatus hash_table_get(const HashTable *table, AdtValue key, AdtValue *out_value)
{
    size_t index = 0;
    HashTableNode *curr = NULL;

    if (!table || !out_value || !table->buckets || table->bucket_count == 0)
    {
        return ADT_ERR_NULL;
    }

    index = hash_table_index(table, key);
    curr = table->buckets[index];
    while (curr)
    {
        if (hash_table_equals(table, curr->key, key))
        {
            *out_value = curr->value;
            return ADT_OK;
        }
        curr = curr->next;
    }

    return ADT_ERR_NOT_FOUND;
}

AdtStatus hash_table_remove(HashTable *table, AdtValue key, AdtValue *out_value)
{
    size_t index = 0;
    HashTableNode *prev = NULL;
    HashTableNode *curr = NULL;

    if (!table || !table->buckets || table->bucket_count == 0)
    {
        return ADT_ERR_NULL;
    }

    index = hash_table_index(table, key);
    curr = table->buckets[index];
    while (curr)
    {
        if (hash_table_equals(table, curr->key, key))
        {
            if (prev)
            {
                prev->next = curr->next;
            }
            else
            {
                table->buckets[index] = curr->next;
            }

            if (out_value)
            {
                *out_value = curr->value;
            }

            free(curr);
            table->size--;
            return ADT_OK;
        }

        prev = curr;
        curr = curr->next;
    }

    return ADT_ERR_NOT_FOUND;
}

AdtStatus hash_table_clear(HashTable *table)
{
    size_t i = 0;

    if (!table || !table->buckets || table->bucket_count == 0)
    {
        return ADT_ERR_NULL;
    }

    for (i = 0; i < table->bucket_count; ++i)
    {
        HashTableNode *curr = table->buckets[i];
        while (curr)
        {
            HashTableNode *next = curr->next;
            free(curr);
            curr = next;
        }
        table->buckets[i] = NULL;
    }

    table->size = 0;
    return ADT_OK;
}

bool hash_table_contains(const HashTable *table, AdtValue key)
{
    AdtValue out = NULL;
    return hash_table_get(table, key, &out) == ADT_OK;
}

size_t hash_table_size(const HashTable *table)
{
    if (!table)
    {
        return 0;
    }

    return table->size;
}
