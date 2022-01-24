/*
 * Implement a variable length list of void pointers. This stores diverse types
 * of data structures that are configured so that the type can he quickly
 * asertained.
 *
 * To use the stack version of this, remember that pop() frees the memory that
 * the data is using. First peek() the data and then when finished with it,
 * then (and only then) use pop() to delete it.
 */
#include "common.h"


str_list_t* create_str_list() {

    str_list_t* lst = _alloc(sizeof(str_list_t));
    lst->cap = 0x01 << 3;
    lst->len = 0;
    lst->list = _alloc_ds(void*, lst->cap);

    return lst;
}

void destroy_str_list(str_list_t* lst) {

    const char *ptr;
    if(lst != NULL) {

        while(NULL != (ptr = peek_str_list(lst)))
            pop_str_list(lst); // free the data and move the TOS.

        _free(lst->list);
        _free(lst);
    }
}

void add_str_list(str_list_t* lst, char* data) {

    if(lst->len+1 > lst->cap) {
        lst->cap <<= 1;
        lst->list = _realloc(lst->list, sizeof(void*)*lst->cap);
    }

    size_t size = strlen(data)+1;
    lst->list[lst->len] = _alloc(size);
    memcpy(lst->list[lst->len], data, size);
    lst->len++;
}

void push_str_list(str_list_t* lst, char* data) {

    add_str_list(lst, data);
}

void pop_str_list(str_list_t* lst) {

    lst->len--;
    if(lst->len >= 0)
        _free(lst->list[lst->len]);
    else
        lst->len = 0;
}

const char* peek_str_list(str_list_t* lst) {

    if((lst->len - 1) >= 0)
        return lst->list[lst->len-1];
    else
        return NULL;
}
