#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "adt_error.h"

/*
 * Tipo valore condiviso tra tutti gli ADT.
 * Ogni ADT gestisce valori tramite puntatore generico.
 */
typedef void *AdtValue;
typedef uint64_t AdtTypeId;

/*
 * Firma comune per inserimento/rimozione generica.
 * I dati entrano/escono in forma "raw" (void* + size) e il chiamante
 * li converte prima/dopo la chiamata.
 */
typedef AdtStatus (*AdtAddFn)(void *adt, const void *in_value, size_t in_size);
typedef AdtStatus (*AdtRemoveFn)(void *adt, void *out_value, size_t out_size);

/*
 * Tabella operazioni base comune.
 * Puoi usarla per registrare le funzioni dell'ADT corrente.
 */
typedef struct AdtOps
{
    AdtAddFn add;
    AdtRemoveFn remove;
} AdtOps;

/*
 * Macro di conversione puntatore-funzione verso la firma comune.
 * Usale solo se la funzione concreta e' compatibile con la firma attesa.
 */
#define ADT_CAST_ADD_FN(fn) ((AdtAddFn)(fn))
#define ADT_CAST_REMOVE_FN(fn) ((AdtRemoveFn)(fn))

/*
 * Registro tipi runtime:
 * - chiave: hash(address + value bytes + value_size)
 * - valore: type_id associato al dato.
 * Quando un dato viene rimosso dall'ADT, va rimosso anche dal registro.
 */
typedef struct AdtTypeRegistry AdtTypeRegistry;

AdtStatus adt_type_registry_create(AdtTypeRegistry **out_registry, size_t bucket_count);
void adt_type_registry_destroy(AdtTypeRegistry *registry);

AdtStatus adt_type_registry_set(
    AdtTypeRegistry *registry,
    const void *address,
    const void *value_bytes,
    size_t value_size,
    AdtTypeId type_id);

AdtStatus adt_type_registry_get(
    const AdtTypeRegistry *registry,
    const void *address,
    const void *value_bytes,
    size_t value_size,
    AdtTypeId *out_type_id);

AdtStatus adt_type_registry_remove(
    AdtTypeRegistry *registry,
    const void *address,
    const void *value_bytes,
    size_t value_size);

/*
 * Varianti utili quando il valore e' stato modificato ma l'indirizzo resta lo stesso.
 */
AdtStatus adt_type_registry_get_by_address(
    const AdtTypeRegistry *registry,
    const void *address,
    AdtTypeId *out_type_id);

AdtStatus adt_type_registry_remove_by_address(
    AdtTypeRegistry *registry,
    const void *address);

AdtStatus adt_type_registry_count_checked(
    const AdtTypeRegistry *registry,
    size_t *out_count);

size_t adt_type_registry_count(const AdtTypeRegistry *registry);

/*
 * Helper cast comodi lato chiamante.
 * Esempio:
 * int x = *ADT_IN_AS(int, in_value);
 * *ADT_OUT_AS(int, out_value) = x;
 */
#define ADT_IN_AS(type, ptr) ((const type *)(ptr))
#define ADT_OUT_AS(type, ptr) ((type *)(ptr))

#endif // COMMON_H
