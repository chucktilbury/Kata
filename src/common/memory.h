/**
 * @file memory.h
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 03-28-2024
 * @copyright Copyright (c) 2024
 */
#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdlib.h>

#define _ALLOC(s) mem_alloc(s)
#define _ALLOC_T(t) (t*)mem_alloc(sizeof(t))
#define _ALLOC_ARRAY(t, n) (t*)mem_alloc(sizeof(t) * (n))
#define _REALLOC(p, s) mem_realloc((p), (s))
#define _REALLOC_T(p, t) (t*)mem_realloc((p), sizeof(t))
#define _REALLOC_ARRAY(p, t, n) (t*)mem_realloc((p), sizeof(t) * (n))
#define _DUP_MEM(p, s) mem_dup((p), (s))
#define _DUP_MEM_T(p, t) (t*)mem_dup((p), sizeof(t))
#define _DUP_MEM_ARRAY(p, t, n) (t*)mem_dup((p), sizeof(t) * (n))
#define _DUP_STR(p) mem_dup_str(p)
#define _FDUP_STR(p, ...) mem_fdup_str(p __VA_OPT__(, ) __VA_ARGS__)
#define _FREE(p) mem_free(((void*)p))

void* mem_alloc(size_t size);
void* mem_realloc(void* ptr, size_t size);
void* mem_dup(void* ptr, size_t size);
char* mem_dup_str(const char* str);
char* mem_fdup_str(const char* str, ...);
void mem_free(void* ptr);

#endif /* _MEMORY_H_ */
