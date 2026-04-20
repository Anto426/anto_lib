#ifndef ADT_ERROR_H
#define ADT_ERROR_H

typedef enum
{
    ADT_OK = 0,
    ADT_ERR_NULL = -1,
    ADT_ERR_EMPTY = -2,
    ADT_ERR_FULL = -3,
    ADT_ERR_ALLOC = -4,
    ADT_ERR_OUT_OF_RANGE = -5
} AdtStatus;

const char *adt_strerror(AdtStatus status);

#endif // ADT_ERROR_H