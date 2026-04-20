#ifndef ADT_ERROR_H
#define ADT_ERROR_H

typedef enum
{
    ADT_OK = 0,

    /* Errori generici */
    ADT_ERR_UNKNOWN = -1,
    ADT_ERR_NULL = -2,
    ADT_ERR_INVALID_ARG = -3,
    ADT_ERR_INVALID_STATE = -4,
    ADT_ERR_NOT_IMPLEMENTED = -5,
    ADT_ERR_NOT_SUPPORTED = -6,

    /* Errori su stato/contenuto degli ADT */
    ADT_ERR_EMPTY = -20,
    ADT_ERR_FULL = -21,
    ADT_ERR_OUT_OF_RANGE = -22,
    ADT_ERR_UNDERFLOW = -23,
    ADT_ERR_OVERFLOW = -24,
    ADT_ERR_DUPLICATE = -25,
    ADT_ERR_NOT_FOUND = -26,

    /* Errori di memoria/risorse */
    ADT_ERR_ALLOC = -40,
    ADT_ERR_REALLOC = -41,
    ADT_ERR_FREE = -42,
    ADT_ERR_CAPACITY = -43,
    ADT_ERR_LIMIT_REACHED = -44,

    /* Errori di integrita' e formato dati */
    ADT_ERR_CORRUPTED = -60,
    ADT_ERR_BAD_FORMAT = -61,
    ADT_ERR_PARSE = -62,
    ADT_ERR_TYPE_MISMATCH = -63,
    ADT_ERR_INVARIANT_BROKEN = -64,

    /* Errori tipici alberi/grafi */
    ADT_ERR_UNBALANCED = -80,
    ADT_ERR_CYCLE_DETECTED = -81,
    ADT_ERR_INVALID_KEY = -82,
    ADT_ERR_PARENT_MISSING = -83,
    ADT_ERR_CHILD_MISSING = -84,

    /* Errori di runtime/sistema */
    ADT_ERR_TIMEOUT = -100,
    ADT_ERR_IO = -101,
    ADT_ERR_PERMISSION = -102,
    ADT_ERR_BUSY = -103,
    ADT_ERR_INTERRUPTED = -104
} AdtStatus;

const char *adt_strerror(AdtStatus status);

#endif // ADT_ERROR_H
