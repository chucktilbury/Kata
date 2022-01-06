#ifndef MEMORY_H
#define MEMORY_H

#define _alloc(s) _alloc_to_macro(__func__, __LINE__, (s))
#define _realloc(p, s) _realloc_to_macro(__func__, __LINE__, (p), (s))
#define _alloc_ds(t, n) _alloc(sizeof(t)*(n))
#define _strdup(s) _strdup_to_macro(__func__, __LINE__, (s))

void* _alloc_to_macro(const char* func, int line, size_t size);
void* _realloc_to_macro(const char* func, int line, void* ptr, size_t size);
const char* _strdup_to_macro(const char* func, int line, const char* str);
void _free(void* ptr);

#endif
