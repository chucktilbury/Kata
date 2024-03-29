/**
 * @file memory.c
 *
 * @brief Low level routines for allocating and freeing memory. Use these
 * routines with the macros provided in the header file.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 03-28-2024
 * @copyright Copyright (c) 2024
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"

#ifdef USE_GC
#include <gc.h>
#endif

/**
 * @brief Wrapper for malloc(). Intended to be used from the macros in
 * the header.
 *
 * @param size
 * @return void*
 *
 */
void* mem_alloc(size_t size) {

    if(size == 0)
        size = 1;

#ifdef USE_GC
    void* ptr = GC_malloc(size);
#else
    void* ptr  = malloc(size);
#endif
    if(ptr == NULL)
        fatal_error("MEMORY: Cannot allocate %lu bytes\n", size);

    memset(ptr, 0, size);
    return ptr;
}

/**
 * @brief Wrapper for realloc().
 *
 * @param ptr
 * @param size
 * @return void*
 *
 */
void* mem_realloc(void* ptr, size_t size) {

#ifdef USE_GC
    void* nptr = GC_realloc(ptr, size);
#else
    void* nptr = realloc(ptr, size);
#endif
    if(nptr == NULL)
        fatal_error("MEMORY: Cannot re-allocate %lu bytes\n", size);

    return nptr;
}

/**
 * @brief Duplicate a hunk of memory.
 *
 * @param ptr
 * @param size
 * @return void*
 *
 */
void* mem_dup(void* ptr, size_t size) {

    void* nptr = mem_alloc(size);

    memcpy(nptr, ptr, size);
    return nptr;
}

/**
 * @brief Duplicate a string.
 *
 * @param str
 * @return char*
 *
 */
char* mem_dup_str(const char* str) {

    if(str != NULL)
        return (char*)mem_dup((void*)str, strlen(str) + 1);
    else
        return (char*)mem_alloc(1);
}

/**
 * @brief Duplicate a string with printf-like formatting.
 *
 * @param str
 * @param ...
 * @return char*
 *
 */
char* mem_fdup_str(const char* str, ...) {

    va_list args;

    va_start(args, str);
    size_t len = vsnprintf(NULL, 0, str, args) + 1;
    va_end(args);

    char* buffer = mem_alloc(len);

    va_start(args, str);
    vsnprintf(buffer, len, str, args);
    va_end(args);

    return buffer;
}

/**
 * @brief Free memory. This is a no-op when using GC.
 *
 * @param ptr
 *
 */
void mem_free(void* ptr) {

#ifndef USE_GC
    if(ptr != NULL)
        free(ptr);
#else
    (void)ptr; // compiler warning
#endif
}
