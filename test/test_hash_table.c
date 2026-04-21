#include "../include/hash_table.h"
#include "../include/util.h"

#include <stdio.h>
#include <string.h>

static uint64_t str_hash(AdtValue key)
{
    const unsigned char *s = (const unsigned char *)key;
    uint64_t hash = 1469598103934665603ULL;

    if (!s)
    {
        return 0;
    }

    while (*s)
    {
        hash ^= (uint64_t)(*s++);
        hash *= 1099511628211ULL;
    }

    return hash;
}

static bool str_equals(AdtValue lhs, AdtValue rhs)
{
    const char *a = (const char *)lhs;
    const char *b = (const char *)rhs;

    if (a == b)
    {
        return true;
    }
    if (!a || !b)
    {
        return false;
    }

    return strcmp(a, b) == 0;
}

int main(void)
{
    HashTable *table = NULL;
    AdtValue value = NULL;
    AdtStatus status = ADT_OK;
    int n1 = 10;
    int n2 = 20;
    int n3 = 30;

    puts("[DEBUG] start test_hash_table");
    status = hash_table_create(&table, 0, str_hash, str_equals);
    debug_status("hash_table_create", status);
    if (status != ADT_OK || !table)
    {
        fprintf(stderr, "hash_table_create failed: %d\n", status);
        return 1;
    }

    status = hash_table_put(table, "one", &n1);
    debug_status("hash_table_put(one)", status);
    status = hash_table_put(table, "two", &n2);
    debug_status("hash_table_put(two)", status);
    if (hash_table_size(table) != 2)
    {
        fprintf(stderr, "hash_table size invalid after put\n");
        hash_table_destroy(table);
        return 1;
    }

    status = hash_table_get(table, "one", &value);
    debug_status("hash_table_get(one)", status);
    if (status != ADT_OK || value != &n1)
    {
        fprintf(stderr, "hash_table_get(one) invalid\n");
        hash_table_destroy(table);
        return 1;
    }

    status = hash_table_put(table, "one", &n3);
    debug_status("hash_table_put(one update)", status);
    status = hash_table_get(table, "one", &value);
    debug_status("hash_table_get(one updated)", status);
    if (status != ADT_OK || value != &n3)
    {
        fprintf(stderr, "hash_table update invalid\n");
        hash_table_destroy(table);
        return 1;
    }

    if (!hash_table_contains(table, "two"))
    {
        fprintf(stderr, "hash_table_contains(two) invalid\n");
        hash_table_destroy(table);
        return 1;
    }

    status = hash_table_remove(table, "two", &value);
    debug_status("hash_table_remove(two)", status);
    if (status != ADT_OK || value != &n2 || hash_table_size(table) != 1)
    {
        fprintf(stderr, "hash_table_remove(two) invalid\n");
        hash_table_destroy(table);
        return 1;
    }

    status = hash_table_remove(table, "missing", &value);
    debug_status("hash_table_remove(missing expected not found)", status);
    if (status != ADT_ERR_NOT_FOUND)
    {
        fprintf(stderr, "expected ADT_ERR_NOT_FOUND, got: %d\n", status);
        hash_table_destroy(table);
        return 1;
    }

    status = hash_table_clear(table);
    debug_status("hash_table_clear", status);
    if (status != ADT_OK || hash_table_size(table) != 0)
    {
        fprintf(stderr, "hash_table_clear invalid\n");
        hash_table_destroy(table);
        return 1;
    }

    hash_table_destroy(table);
    puts("test_hash_table OK");
    return 0;
}
