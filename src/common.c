#include "../include/common.h"

#include <stdlib.h>

typedef struct AdtTypeEntry
{
    const void *address;
    uint64_t value_hash;
    size_t value_size;
    AdtTypeId type_id;
    struct AdtTypeEntry *next;
} AdtTypeEntry;

struct AdtTypeRegistry
{
    size_t bucket_count;
    size_t count;
    AdtTypeEntry **buckets;
};

static AdtStatus adt_registry_validate(const AdtTypeRegistry *registry)
{
    if (!registry)
    {
        return ADT_ERR_NULL;
    }

    if (registry->bucket_count == 0 || !registry->buckets)
    {
        return ADT_ERR_CORRUPTED;
    }

    return ADT_OK;
}

static uint64_t adt_hash_bytes(const void *data, size_t len)
{
    const unsigned char *bytes = (const unsigned char *)data;
    uint64_t hash = 1469598103934665603ULL;
    size_t i = 0;

    for (i = 0; i < len; ++i)
    {
        hash ^= (uint64_t)bytes[i];
        hash *= 1099511628211ULL;
    }

    return hash;
}

static uint64_t adt_hash_ptr(const void *ptr)
{
    uint64_t x = (uint64_t)(uintptr_t)ptr;

    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;

    return x;
}

static uint64_t adt_value_fingerprint(const void *value_bytes, size_t value_size)
{
    if (!value_bytes || value_size == 0)
    {
        return 0x9e3779b97f4a7c15ULL;
    }

    return adt_hash_bytes(value_bytes, value_size);
}

static uint64_t adt_key_hash(const void *address, uint64_t value_hash, size_t value_size)
{
    uint64_t hash = 1469598103934665603ULL;

    hash ^= adt_hash_ptr(address);
    hash *= 1099511628211ULL;
    hash ^= value_hash;
    hash *= 1099511628211ULL;
    hash ^= (uint64_t)value_size;
    hash *= 1099511628211ULL;

    return hash;
}

static size_t adt_bucket_index(const AdtTypeRegistry *registry, uint64_t key_hash)
{
    return (size_t)(key_hash % (uint64_t)registry->bucket_count);
}

static AdtTypeEntry *adt_find_by_address(
    const AdtTypeRegistry *registry,
    const void *address,
    size_t *out_bucket_index,
    AdtTypeEntry **out_prev)
{
    size_t i = 0;

    if (!registry || !address || !registry->buckets || registry->bucket_count == 0)
    {
        return NULL;
    }

    for (i = 0; i < registry->bucket_count; ++i)
    {
        AdtTypeEntry *prev = NULL;
        AdtTypeEntry *curr = registry->buckets[i];

        while (curr)
        {
            if (curr->address == address)
            {
                if (out_bucket_index)
                {
                    *out_bucket_index = i;
                }
                if (out_prev)
                {
                    *out_prev = prev;
                }
                return curr;
            }

            prev = curr;
            curr = curr->next;
        }
    }

    return NULL;
}

AdtStatus adt_type_registry_create(AdtTypeRegistry **out_registry, size_t bucket_count)
{
    AdtTypeRegistry *registry = NULL;

    if (!out_registry)
    {
        return ADT_ERR_NULL;
    }

    if (bucket_count == 0)
    {
        bucket_count = 64;
    }

    if (bucket_count > (SIZE_MAX / sizeof(AdtTypeEntry *)))
    {
        *out_registry = NULL;
        return ADT_ERR_OVERFLOW;
    }

    registry = (AdtTypeRegistry *)malloc(sizeof(AdtTypeRegistry));
    if (!registry)
    {
        *out_registry = NULL;
        return ADT_ERR_ALLOC;
    }

    registry->buckets = (AdtTypeEntry **)calloc(bucket_count, sizeof(AdtTypeEntry *));
    if (!registry->buckets)
    {
        free(registry);
        *out_registry = NULL;
        return ADT_ERR_ALLOC;
    }

    registry->bucket_count = bucket_count;
    registry->count = 0;
    *out_registry = registry;

    return ADT_OK;
}

void adt_type_registry_destroy(AdtTypeRegistry *registry)
{
    size_t i = 0;

    if (!registry)
    {
        return;
    }

    if (registry->buckets && registry->bucket_count > 0)
    {
        for (i = 0; i < registry->bucket_count; ++i)
        {
            AdtTypeEntry *curr = registry->buckets[i];
            while (curr)
            {
                AdtTypeEntry *next = curr->next;
                free(curr);
                curr = next;
            }
        }

        free(registry->buckets);
    }

    free(registry);
}

AdtStatus adt_type_registry_set(
    AdtTypeRegistry *registry,
    const void *address,
    const void *value_bytes,
    size_t value_size,
    AdtTypeId type_id)
{
    uint64_t value_hash = 0;
    uint64_t key_hash = 0;
    size_t bucket = 0;
    AdtTypeEntry *entry = NULL;
    AdtStatus remove_status = ADT_OK;
    AdtStatus validate_status = ADT_OK;

    if (!registry || !address)
    {
        return ADT_ERR_NULL;
    }

    if (value_size > 0 && !value_bytes)
    {
        return ADT_ERR_INVALID_ARG;
    }

    validate_status = adt_registry_validate(registry);
    if (validate_status != ADT_OK)
    {
        return validate_status;
    }

    if (registry->count == SIZE_MAX)
    {
        return ADT_ERR_OVERFLOW;
    }

    /*
     * Un indirizzo rappresenta un solo dato registrato.
     * Se gia' presente, lo rimpiazziamo con il nuovo hash/tipo.
     */
    remove_status = adt_type_registry_remove_by_address(registry, address);
    if (remove_status != ADT_OK && remove_status != ADT_ERR_NOT_FOUND)
    {
        return remove_status;
    }

    value_hash = adt_value_fingerprint(value_bytes, value_size);
    key_hash = adt_key_hash(address, value_hash, value_size);
    bucket = adt_bucket_index(registry, key_hash);
    if (bucket >= registry->bucket_count)
    {
        return ADT_ERR_CORRUPTED;
    }

    entry = (AdtTypeEntry *)malloc(sizeof(AdtTypeEntry));
    if (!entry)
    {
        return ADT_ERR_ALLOC;
    }

    entry->address = address;
    entry->value_hash = value_hash;
    entry->value_size = value_size;
    entry->type_id = type_id;
    entry->next = registry->buckets[bucket];
    registry->buckets[bucket] = entry;
    registry->count++;

    return ADT_OK;
}

AdtStatus adt_type_registry_get(
    const AdtTypeRegistry *registry,
    const void *address,
    const void *value_bytes,
    size_t value_size,
    AdtTypeId *out_type_id)
{
    uint64_t value_hash = 0;
    uint64_t key_hash = 0;
    size_t bucket = 0;
    AdtTypeEntry *curr = NULL;
    AdtStatus validate_status = ADT_OK;

    if (!registry || !address || !out_type_id)
    {
        return ADT_ERR_NULL;
    }

    if (value_size > 0 && !value_bytes)
    {
        return ADT_ERR_INVALID_ARG;
    }

    validate_status = adt_registry_validate(registry);
    if (validate_status != ADT_OK)
    {
        return validate_status;
    }

    value_hash = adt_value_fingerprint(value_bytes, value_size);
    key_hash = adt_key_hash(address, value_hash, value_size);
    bucket = adt_bucket_index(registry, key_hash);
    if (bucket >= registry->bucket_count)
    {
        return ADT_ERR_CORRUPTED;
    }

    curr = registry->buckets[bucket];
    while (curr)
    {
        if (curr->address == address &&
            curr->value_hash == value_hash &&
            curr->value_size == value_size)
        {
            *out_type_id = curr->type_id;
            return ADT_OK;
        }
        curr = curr->next;
    }

    /* Se esiste l'indirizzo ma hash diverso, il valore e' cambiato. */
    if (adt_find_by_address(registry, address, NULL, NULL))
    {
        return ADT_ERR_TYPE_MISMATCH;
    }

    return ADT_ERR_NOT_FOUND;
}

AdtStatus adt_type_registry_remove(
    AdtTypeRegistry *registry,
    const void *address,
    const void *value_bytes,
    size_t value_size)
{
    uint64_t value_hash = 0;
    uint64_t key_hash = 0;
    size_t bucket = 0;
    AdtTypeEntry *prev = NULL;
    AdtTypeEntry *curr = NULL;
    AdtStatus validate_status = ADT_OK;

    if (!registry || !address)
    {
        return ADT_ERR_NULL;
    }

    if (value_size > 0 && !value_bytes)
    {
        return ADT_ERR_INVALID_ARG;
    }

    validate_status = adt_registry_validate(registry);
    if (validate_status != ADT_OK)
    {
        return validate_status;
    }

    value_hash = adt_value_fingerprint(value_bytes, value_size);
    key_hash = adt_key_hash(address, value_hash, value_size);
    bucket = adt_bucket_index(registry, key_hash);
    if (bucket >= registry->bucket_count)
    {
        return ADT_ERR_CORRUPTED;
    }

    curr = registry->buckets[bucket];
    while (curr)
    {
        if (curr->address == address &&
            curr->value_hash == value_hash &&
            curr->value_size == value_size)
        {
            if (registry->count == 0)
            {
                return ADT_ERR_CORRUPTED;
            }

            if (prev)
            {
                prev->next = curr->next;
            }
            else
            {
                registry->buckets[bucket] = curr->next;
            }

            free(curr);
            registry->count--;
            return ADT_OK;
        }

        prev = curr;
        curr = curr->next;
    }

    /*
     * Fallback utile quando il dato e' stato cambiato dopo la registrazione:
     * in quel caso rimuoviamo per solo indirizzo.
     */
    return adt_type_registry_remove_by_address(registry, address);
}

AdtStatus adt_type_registry_get_by_address(
    const AdtTypeRegistry *registry,
    const void *address,
    AdtTypeId *out_type_id)
{
    AdtTypeEntry *entry = NULL;
    AdtStatus validate_status = ADT_OK;

    if (!registry || !address || !out_type_id)
    {
        return ADT_ERR_NULL;
    }

    validate_status = adt_registry_validate(registry);
    if (validate_status != ADT_OK)
    {
        return validate_status;
    }

    entry = adt_find_by_address(registry, address, NULL, NULL);
    if (!entry)
    {
        return ADT_ERR_NOT_FOUND;
    }

    *out_type_id = entry->type_id;
    return ADT_OK;
}

AdtStatus adt_type_registry_remove_by_address(
    AdtTypeRegistry *registry,
    const void *address)
{
    size_t bucket_index = 0;
    AdtTypeEntry *prev = NULL;
    AdtTypeEntry *entry = NULL;
    AdtStatus validate_status = ADT_OK;

    if (!registry || !address)
    {
        return ADT_ERR_NULL;
    }

    validate_status = adt_registry_validate(registry);
    if (validate_status != ADT_OK)
    {
        return validate_status;
    }

    entry = adt_find_by_address(registry, address, &bucket_index, &prev);
    if (!entry)
    {
        return ADT_ERR_NOT_FOUND;
    }

    if (registry->count == 0)
    {
        return ADT_ERR_CORRUPTED;
    }

    if (prev)
    {
        prev->next = entry->next;
    }
    else
    {
        registry->buckets[bucket_index] = entry->next;
    }

    free(entry);
    registry->count--;
    return ADT_OK;
}

AdtStatus adt_type_registry_count_checked(
    const AdtTypeRegistry *registry,
    size_t *out_count)
{
    AdtStatus validate_status = ADT_OK;

    if (!out_count)
    {
        return ADT_ERR_NULL;
    }

    validate_status = adt_registry_validate(registry);
    if (validate_status != ADT_OK)
    {
        return validate_status;
    }

    *out_count = registry->count;
    return ADT_OK;
}

size_t adt_type_registry_count(const AdtTypeRegistry *registry)
{
    size_t count = 0;
    if (adt_type_registry_count_checked(registry, &count) != ADT_OK)
    {
        return 0;
    }

    return count;
}
