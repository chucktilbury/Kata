
#include <stdarg.h>

#include "common.h"


string* create_string() {

    string* str = _alloc(sizeof(string));
    str->cap = 0x01 << 3;
    str->len = 0;
    str->str = _alloc(str->cap);
    return str;
}

void add_string_char(string* sptr, int ch) {

    if(sptr->len + 2 >= sptr->cap) {
        sptr->cap <<= 1;
        sptr->str = _realloc(sptr->str, sptr->cap);
    }

    sptr->str[sptr->len] = (char)ch;
    sptr->len++;
    sptr->str[sptr->len] = '\0';
}

void add_string_strg(string* str, const char* s) {

    for(int i = 0; s[i] != '\0'; i++)
        add_string_char(str, s[i]);
}

const char* get_string(string* strg) {

    return strg->str;
}

void destroy_string(string* sptr) {

    _free(sptr->str);
    _free(sptr);
}

/*
 * Return the variable list of strings concatenated into a single allocated
 * buffer.
 */
const char* cat_strings(int num, ...) {

    va_list args;
    string* str = create_string();

    va_start(args, num);
    for(int i = 0; i < num; i++) {
        add_string_strg(str, va_arg(args, const char*));
    }
    va_end(args);

    const char* s = _strdup(str->str);
    destroy_string(str);

    return s;
}
