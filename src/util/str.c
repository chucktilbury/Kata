#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

// Join a list where the str is between the elements of the list.
Str* join_str_list(StrList* lst, const char* str) {

    Str* s = create_string(NULL);
    Str* tmp;

    StrListIter* sli = init_string_list_iterator(lst);
    add_string_Str(s, iterate_string_list(sli));
    while(NULL != (tmp = iterate_string_list(sli))) {
        add_string_str(s, str);
        add_string_Str(s, tmp);
    }

    return s;
}

Str* create_string(const char* str) {

    Str* ptr = create_list(sizeof(char));

    if(str != NULL)
        add_string_str(ptr, str);

    return ptr;
}

Str* create_string_fmt(const char* str, ...) {

    assert(str != NULL);
    va_list args;

    va_start(args, str);
    size_t len = vsnprintf(NULL, 0, str, args);
    va_end(args);

    char* ptr = _ALLOC(len + 1);

    va_start(args, str);
    vsnprintf(ptr, len + 1, str, args);
    va_end(args);

    Str* retv = create_string(ptr);
    _FREE(ptr);

    return retv;
}

void destroy_string(Str* ptr) {

    if(ptr != NULL) {
        destroy_list(ptr);
        _FREE(ptr);
    }
}

void add_string_char(Str* ptr, int ch) {

    append_list(ptr, &ch);
}

void add_string_str(Str* ptr, const char* str) {

    for(const char* ch = str; *ch != '\0'; ch++)
        add_string_char(ptr, *ch);
}

void add_string_fmt(Str* ptr, const char* str, ...) {

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

StrIter* init_string_iter(Str* ptr) {
    return init_list_iterator(ptr);
}

int iterate_string(StrIter* ptr) {
    int ch;
    if(iterate_list(ptr, &ch))
        return ch;
    else
        return 0;
}

const char* raw_string(Str* ptr) {

    int ch = 0;
    push_list(ptr, &ch);
    pop_list(ptr, NULL);

    return (const char*)raw_list(ptr);
}

int comp_string(Str* s1, Str* s2) {
    return strcmp(raw_string(s1), raw_string(s2));
}

int comp_string_const(Str* s1, const char* s2) {
    return strcmp(raw_string(s1), s2);
}

Str* copy_string(Str* str) {
    return create_string(raw_string(str));
}

void truncate_string(Str* str, int index) {

    int val = 0;
    write_list(str, index, &val);
}

void clear_string(Str* str) {

    truncate_string(str, 0);
    clear_list(str);
}

int length_string(Str* str) {

    return length_list(str);
}

void add_string_Str(Str* ptr, Str* str) {

    add_string_str(ptr, raw_string(str));
}

void print_string(FILE* fp, Str* str) {

    fprintf(fp, "%s", raw_string(str));
}

void printf_string(FILE* fp, Str* str, ...) {

    va_list args;

    va_start(args, str);
    vfprintf(fp, raw_string(str), args);
    va_end(args);
}

Str* upcase_string(Str* str) {

    char* ptr = _DUP_STR(raw_string(str));

    while(*ptr != '\0') {
        *ptr = toupper(*ptr);
        ptr++;
    }

    Str* s = create_string(ptr);
    _FREE(ptr);

    return s;
}

Str* downcase_string(Str* str) {

    char* ptr = _DUP_STR(raw_string(str));

    while(*ptr != '\0') {
        *ptr = tolower(*ptr);
        ptr++;
    }

    Str* s = create_string(ptr);
    _FREE(ptr);

    return s;
}
