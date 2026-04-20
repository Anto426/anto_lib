#include "../include/adt_error.h"

const char *adt_strerror(AdtStatus status)
{
    switch (status)
    {
    case ADT_OK:
        return "Operation completed";

    case ADT_ERR_UNKNOWN:
        return "Unknown error";
    case ADT_ERR_NULL:
        return "Null pointer";
    case ADT_ERR_INVALID_ARG:
        return "Invalid argument";
    case ADT_ERR_INVALID_STATE:
        return "Invalid state";
    case ADT_ERR_NOT_IMPLEMENTED:
        return "Function not implemented";
    case ADT_ERR_NOT_SUPPORTED:
        return "Operation not supported";

    case ADT_ERR_EMPTY:
        return "Data structure is empty";
    case ADT_ERR_FULL:
        return "Data structure is full";
    case ADT_ERR_OUT_OF_RANGE:
        return "Index out of range";
    case ADT_ERR_UNDERFLOW:
        return "Underflow";
    case ADT_ERR_OVERFLOW:
        return "Overflow";
    case ADT_ERR_DUPLICATE:
        return "Duplicate element";
    case ADT_ERR_NOT_FOUND:
        return "Element not found";

    case ADT_ERR_ALLOC:
        return "Memory allocation failed";
    case ADT_ERR_REALLOC:
        return "Memory reallocation failed";
    case ADT_ERR_FREE:
        return "Memory deallocation error";
    case ADT_ERR_CAPACITY:
        return "Invalid or exceeded capacity";
    case ADT_ERR_LIMIT_REACHED:
        return "Maximum limit reached";

    case ADT_ERR_CORRUPTED:
        return "Corrupted data";
    case ADT_ERR_BAD_FORMAT:
        return "Invalid data format";
    case ADT_ERR_PARSE:
        return "Parsing error";
    case ADT_ERR_TYPE_MISMATCH:
        return "Type mismatch";
    case ADT_ERR_INVARIANT_BROKEN:
        return "Data structure invariant violated";

    case ADT_ERR_UNBALANCED:
        return "Unbalanced tree";
    case ADT_ERR_CYCLE_DETECTED:
        return "Cycle detected";
    case ADT_ERR_INVALID_KEY:
        return "Invalid key";
    case ADT_ERR_PARENT_MISSING:
        return "Missing parent node";
    case ADT_ERR_CHILD_MISSING:
        return "Missing child node";

    case ADT_ERR_TIMEOUT:
        return "Timeout";
    case ADT_ERR_IO:
        return "Input/output error";
    case ADT_ERR_PERMISSION:
        return "Permission denied";
    case ADT_ERR_BUSY:
        return "Resource busy";
    case ADT_ERR_INTERRUPTED:
        return "Operation interrupted";

    default:
        return "Unrecognized error code";
    }
}
