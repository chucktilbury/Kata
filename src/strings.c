
#include <stdlib.h>

#include "strings.h"
#include "memory.h"

typedef struct {
    char* str;
    size_t len;
    size_t cap;
} _string;

string create_string() {

    _string* str = _alloc(sizeof(_string));
    str->cap = 0x01 << 3;
    str->len = 0;
    str->str = _alloc(str->cap);
    return str;
}

void add_string_char(string str, int ch) {

    _string* sptr = (_string*)str;

    if(sptr->len + 2 >= sptr->cap) {
        sptr->cap <<= 1;
        sptr->str = _realloc(sptr->str, sptr->cap);
    }

    sptr->str[sptr->len] = (char)ch;
    sptr->len++;
    sptr->str[sptr->len] = '\0';
}

void add_string_strg(string str, const char* s) {

    for(int i = 0; s[i] != '\0'; i++)
        add_string_char((_string*)str, s[i]);
}

const char* get_string(string strg) {

    return ((_string*)strg)->str;
}

void destroy_string(string str) {

    _string* sptr = (_string*)str;
    _free(sptr->str);
    _free(sptr);
}
