#include "../include/util.h"

#include <stdio.h>

void debug_status(const char *label, AdtStatus status)
{
    printf("[DEBUG] %s -> %d (%s)\n", label, status, adt_strerror(status));
}
