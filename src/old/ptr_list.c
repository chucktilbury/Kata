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


ptr_list_t* create_ptr_list() {

    ptr_list_t* lst = _alloc(sizeof(ptr_list_t));
    lst->cap = 0x01 << 3;
    lst->len = 0;
    lst->list = _alloc_ds(void*, lst->cap);

    return lst;
}

void destroy_ptr_list(ptr_list_t* lst) {

    void *ptr;
    if(lst != NULL) {

        while(NULL != (ptr = peek_ptr_list(lst)))
            pop_ptr_list(lst); // free the data and move the TOS.

        _free(lst->list);
        _free(lst);
    }
}

void add_ptr_list(ptr_list_t* lst, void* data, size_t size) {

    if(lst->len+1 > lst->cap) {
        lst->cap <<= 1;
        lst->list = _realloc(lst->list, sizeof(void*)*lst->cap);
    }

    lst->list[lst->len] = _alloc(size);
    memcpy(lst->list[lst->len], data, size);
    lst->len++;
}

void push_ptr_list(ptr_list_t* lst, void* data, size_t size) {

    add_ptr_list(lst, data, size);
}

void pop_ptr_list(ptr_list_t* lst) {

    lst->len--;
    if(lst->len >= 0)
        _free(lst->list[lst->len]);
    else
        lst->len = 0;
}

void* peek_ptr_list(ptr_list_t* lst) {

    if((lst->len - 1) >= 0)
        return lst->list[lst->len-1];
    else
        return NULL;
}
