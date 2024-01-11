#ifndef STR_LIST_H
#define STR_LIST_H

typedef struct {
    char** list;
    int cap;
    int len;
} str_list_t;

str_list_t* create_str_list();
void destroy_str_list(str_list_t* lst);
void add_str_list(str_list_t* lst, char* data);
void push_str_list(str_list_t* lst, char* data);
void pop_str_list(str_list_t* lst);
const char* peek_str_list(str_list_t* lst);

#endif
