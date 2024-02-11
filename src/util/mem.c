
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

#ifdef USE_GC
#include <gc.h>
#endif

void* mem_alloc(size_t size) {

    if(size == 0)
        size = 1;

#ifdef USE_GC
    void* ptr = GC_malloc(size);
#else
    void* ptr  = malloc(size);
#endif
    if(ptr == NULL)
        RAISE(MEMORY_ERROR, "MEMORY: Cannot allocate %lu bytes\n", size);

    memset(ptr, 0, size);
    return ptr;
}

void* mem_realloc(void* ptr, size_t size) {

#ifdef USE_GC
    void* nptr = GC_realloc(ptr, size);
#else
    void* nptr = realloc(ptr, size);
#endif
    if(nptr == NULL)
        RAISE(MEMORY_ERROR, "MEMORY: Cannot re-allocate %lu bytes\n", size);

    return nptr;
}

void* mem_dup(void* ptr, size_t size) {

    void* nptr = mem_alloc(size);

    memcpy(nptr, ptr, size);
    return nptr;
}

char* mem_dup_str(const char* str) {

    if(str != NULL)
        return (char*)mem_dup((void*)str, strlen(str) + 1);
    else
        return (char*)mem_alloc(1);
}

char* mem_fdup_str(const char* str, ...) {

    va_list args;

    va_start(args, str);
    size_t len = vsnprintf(NULL, 0, str, args);
    va_end(args);

    char* buffer = mem_alloc(len + 1);

    va_start(args, str);
    vsnprintf(buffer, len, str, args);
    va_end(args);

    return buffer;
}


void mem_free(void* ptr) {

#ifndef USE_GC
    if(ptr != NULL)
        free(ptr);
#else
    (void)ptr; // compiler warning
#endif
}
