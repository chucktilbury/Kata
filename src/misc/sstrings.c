#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "sstrings.h"
#include "memory.h"

/**
 * @brief Append a string to a buffer
 *
 * @param ptr
 * @param buffer
 * @param size
 *
 */
static void append_string(String* ptr, const char* buffer, size_t size) {

    if(ptr->len+(int)size > ptr->cap) {
        while(ptr->len+(int)size > ptr->cap)
            ptr->cap <<= 1;
        ptr->buffer = _REALLOC(ptr->buffer, ptr->cap);
    }

    memcpy(&ptr->buffer[ptr->len], buffer, size);
    ptr->len += size;
    ptr->buffer[ptr->len] = 0;
}


/**
 * @brief Join a list where the str is between the elements of the list.
 *
 * @param lst
 * @param str
 * @return String*
 *
 */
String* join_string_list(StrList* lst, const char* str) {

    String* s = create_string(NULL);
    String* tmp;
    void* mark;

    LLFOR(lst, tmp, mark) {
        add_string_Str(s, tmp);
        add_string_str(s, str);
    }

    return s;
}

/**
 * @brief Create a string object
 *
 * @param str
 * @return String*
 *
 */
String* create_string(const char* str) {

    String* ptr = _ALLOC_T(String);
    ptr->len = 0;
    ptr->cap = 0x01 << 3;
    ptr->buffer = _ALLOC(ptr->cap);

    if(str != NULL)
        add_string_str(ptr, str);

    return ptr;
}

/**
 * @brief Create a string fmt object
 *
 * @param str
 * @param ...
 * @return String*
 *
 */
String* create_string_fmt(const char* str, ...) {

    assert(str != NULL);
    va_list args;

    va_start(args, str);
    size_t len = vsnprintf(NULL, 0, str, args);
    va_end(args);

    char* ptr = _ALLOC(len + 1);

    va_start(args, str);
    vsnprintf(ptr, len + 1, str, args);
    va_end(args);

    String* retv = create_string(ptr);
    _FREE(ptr);

    return retv;
}

/**
 * @brief Destroy a string. current a no-op because we are using garbage
 * collection.
 *
 * @param ptr
 *
 */
void destroy_string(String* ptr) {

    assert(ptr != NULL);
}

/**
 * @brief Add a single character to the string.
 *
 * @param ptr
 * @param ch
 *
 */
void add_string_char(String* ptr, int ch) {

    append_string(ptr, (const char*)(&ch), 1);
}

/**
 * @brief Add an array of chars to the string.
 *
 * @param ptr
 * @param str
 *
 */
void add_string_str(String* ptr, const char* str) {

    append_string(ptr, str, strlen(str));
}

/**
 * @brief Add an array of chars to the string with formatting.
 *
 * @param ptr
 * @param str
 * @param ...
 *
 */
void add_string_fmt(String* ptr, const char* str, ...) {

    assert(ptr != NULL);
    assert(str != NULL);

    va_list args;

    va_start(args, str);
    size_t len = vsnprintf(NULL, 0, str, args);
    va_end(args);

    char* spt = _ALLOC(len + 1);

    va_start(args, str);
    vsnprintf(spt, len + 1, str, args);
    va_end(args);

    add_string_str(ptr, spt);
    _FREE(spt);
}

/**
 * @brief Iterate the characters in the string.
 *
 * @param ptr
 * @param mark
 * @return int
 *
 */
int iter_string(String* ptr, int* mark) {

    int ch = 0;

    if(*mark >= 0)
        ch = ptr->buffer[*mark];

    if(ch != 0)
        (*mark)++;
    else
        *mark = -1;

    return ch;
}

/**
 * @brief Return the raw string.
 *
 * @param ptr
 * @return const char*
 *
 */
const char* raw_string(String* ptr) {

    return (const char*)ptr->buffer;
}

/**
 * @brief Compare two String data structures.
 *
 * @param s1
 * @param s2
 * @return int
 *
 */
int comp_string(String* s1, String* s2) {
    return strcmp(raw_string(s1), raw_string(s2));
}

/**
 * @brief Compare a String to a raw string.
 *
 * @param s1
 * @param s2
 * @return int
 *
 */
int comp_string_const(String* s1, const char* s2) {
    return strcmp(raw_string(s1), s2);
}

/**
 * @brief Create a copy of a String.
 *
 * @param str
 * @return String*
 *
 */
String* copy_string(String* str) {
    return create_string(raw_string(str));
}

/**
 * @brief Clip a string at a given index. Does not change the capacity.
 *
 * @param str
 * @param index
 *
 */
void truncate_string(String* str, int index) {

    if(index < str->len) {
        str->buffer[index] = 0;
        str->len = index;
    }
    // else fail silently
}

/**
 * @brief Clear a string data structure.
 *
 * @param str
 *
 */
void clear_string(String* str) {

    truncate_string(str, 0);
}

/**
 * @brief Return the length of a String.
 *
 * @param str
 * @return int
 *
 */
int length_string(String* str) {

    return str->len;
}

/**
 * @brief Append a String to an existing String.
 *
 * @param ptr
 * @param str
 *
 */
void add_string_Str(String* ptr, String* str) {

    add_string_str(ptr, raw_string(str));
}

/**
 * @brief Print a string to the given stream.
 *
 * @param fp
 * @param str
 *
 */
void print_string(FILE* fp, String* str) {

    fprintf(fp, "%s", raw_string(str));
}

/**
 * @brief Print a String to the given stream with formatting.
 *
 * @param fp
 * @param str
 * @param ...
 *
 */
void printf_string(FILE* fp, String* str, ...) {

    va_list args;

    va_start(args, str);
    vfprintf(fp, raw_string(str), args);
    va_end(args);
}

/**
 * @brief Make a copy of the string with all of the characters upper cased.
 *
 * @param str
 * @return String*
 *
 */
String* upcase_string(String* str) {

    String* s = copy_string(str);
    char* ptr = (char*)s->buffer;

    while(*ptr != '\0') {
        *ptr = toupper(*ptr);
        ptr++;
    }

    return s;
}

/**
 * @brief Make a copy of the String with all of the characters lower cased.
 *
 * @param str
 * @return String*
 *
 */
String* downcase_string(String* str) {

    String* s = copy_string(str);
    char* ptr = (char*)s->buffer;

    while(*ptr != '\0') {
        *ptr = tolower(*ptr);
        ptr++;
    }

    return s;
}

/**
 * @brief Create a string list object
 *
 * @return StrList*
 *
 */
StrList* create_string_list() {
    return create_link_list();
}

/**
 * @brief Destroy a string list. Currently a no-op because we are
 * using garbage collection.
 *
 * @param lst
 *
 */
void destroy_string_list(StrList* lst) {
    destroy_link_list(lst);
}

/**
 * @brief Add a string to the string list.
 *
 * @param lst
 * @param str
 *
 */
void add_string_list(StrList* lst, String* str) {
    //printf("add list: %s\n", str->buffer);
    append_link_list(lst, str);
}

/**
 * @brief Iterate the string list.
 *
 * @param ptr
 * @param mark
 * @return String*
 *
 */
String* iterate_string_list(StrList* ptr, void** mark) {
    return iter_link_list(ptr, mark);
}

/**
 * @brief Push the string pointer on the list as a stack.
 *
 * @param lst
 * @param str
 *
 */
void push_string_list(StrList* lst, String* str) {
    //printf("push list: %s\n", str->buffer);
    push_link_list(lst, copy_string(str));
}

/**
 * @brief Peek the most recently pushed item.
 *
 * @param lst
 * @return String*
 *
 */
String* peek_string_list(StrList* lst) {

    //printf("peek list: %s\n", (const char*)lst->first->data);
    //printf("peek list: %s\n", (const char*)lst->last->data);
    return peek_link_list(lst);
}

/**
 * @brief Pop the string list.
 *
 * @param lst
 * @return String*
 *
 */
String* pop_string_list(StrList* lst) {

    return pop_link_list(lst);
}
