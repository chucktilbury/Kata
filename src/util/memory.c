
#include <stdlib.h>
#include <string.h>

#include "errors.h"

void* _alloc_to_macro(const char* str, int line, size_t size) {

    void* p = malloc(size);
    if(p == NULL)
        fatal("%s: %d: cannot allocate %lu bytes", str, line, size);

    memset(p, 0, size);
    return p;
}

void* _realloc_to_macro(const char* str, int line, void* ptr, size_t size) {

    void* p = realloc(ptr, size);
    if(p == NULL)
        fatal("%s: %d: cannot re-allocate %lu bytes", str, line, size);

    return p;
}

void _free(void* ptr) {

    if(ptr != NULL)
        free(ptr);
}

const char* _strdup_to_macro(const char* func, int line, const char* str) {

    size_t size = strlen(str) + 1;
    void* p = malloc(size);
    if(p == NULL)
        fatal("%s: %d: cannot allocate %lu bytes", func, line, size);

    memcpy(p, str, size);
    return p;
}
