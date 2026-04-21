#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "common.h"

typedef struct HashTable HashTable;

typedef uint64_t (*HashTableHashFn)(AdtValue key);
typedef bool (*HashTableEqualsFn)(AdtValue lhs, AdtValue rhs);

AdtStatus hash_table_create(
    HashTable **out_table,
    size_t bucket_count,
    HashTableHashFn hash_fn,
    HashTableEqualsFn equals_fn);
void hash_table_destroy(HashTable *table);

AdtStatus hash_table_put(HashTable *table, AdtValue key, AdtValue value);
AdtStatus hash_table_get(const HashTable *table, AdtValue key, AdtValue *out_value);
AdtStatus hash_table_remove(HashTable *table, AdtValue key, AdtValue *out_value);

AdtStatus hash_table_clear(HashTable *table);

bool hash_table_contains(const HashTable *table, AdtValue key);
size_t hash_table_size(const HashTable *table);

#endif // HASH_TABLE_H
