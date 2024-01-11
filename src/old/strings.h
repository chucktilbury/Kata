#ifndef STRINGS_H
#define STRINGS_H

typedef struct {
    char* str;
    size_t len;
    size_t cap;
} string;


string* create_string();
void add_string_char(string* str, int ch);
void add_string_strg(string* str, const char* s);
const char* get_string(string* str);
void destroy_string(string* str);
const char* cat_strings(int num, ...);

#endif
