/**
 * @file strings.h
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 03-29-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __STRINGS_H__
#define __STRINGS_H__

#include "link_list.h"
#include <stdio.h>

typedef struct {
    unsigned char* buffer;
    int len;
    int cap;
} String;

typedef LinkList StrList;

String* join_string_list(StrList* lst, const char* str);
String* copy_string(String* str);
String* create_string(const char* str);
String* create_string_fmt(const char* str, ...);
void destroy_string(String* ptr);
void add_string_char(String* ptr, int ch);
void add_string_str(String* ptr, const char* str);
void add_string_fmt(String* ptr, const char* str, ...);

int iterate_string(StrList* ptr, void** mark);

const char* raw_string(String* ptr);
int comp_string(String* s1, String* s2);
int comp_string_const(String* s1, const char* s2);

// TODO: Insert sub-string and delete range. The list functionality will not
// need to support this, so this library will need to manipulate the raw
// buffer. It may be worth it to put a hook in the lists to swap or replace
// the buffer.
void truncate_string(String* str, int index);
void clear_string(String* str);
int length_string(String* str);
void add_string_Str(String* ptr, String* str);
String* upcase_string(String* str);
String* downcase_string(String* str);

void print_string(FILE* fp, String* str);
void printf_string(FILE* fp, String* str, ...);

StrList* create_string_list();
void destroy_string_list(StrList* lst);
void add_string_list(StrList* lst, String* str);
String* iterate_string_list(StrList* ptr, void** mark);
void push_string_list(StrList* lst, String* str);
String* peek_string_list(StrList* lst);
String* pop_string_list(StrList* lst);

#endif /* __STRINGS_H__ */
