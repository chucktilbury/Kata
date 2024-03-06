/**
 * @file llist.c
 * 
 * @brief Implement opaque generic single linked lists.
 * 
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 03-04-2024
 * @copyright Copyright (c) 2024
 */
#include "util.h"

typedef struct _llist_elem_ {
    void* data;
    struct _llist_elem_* next;
} llist_elem;

typedef struct {
    llist_elem* head;
    llist_elem* crnt;
    llist_elem* tail;
    bool changed;
    int length;
} llist;

void* create_llist() {

    llist* ll = _ALLOC_T(llist);
    ll->head = NULL;
    ll->tail = NULL;
    ll->crnt = NULL;
    ll->changed = true;
    ll->length = 0;

    return (void*)ll;
}

void append_llist(LList ll, void* data) {

    assert(ll != NULL);
    assert(data != NULL);
    llist* lst = (llist*)ll;

    llist_elem* elem = _ALLOC_T(llist_elem);
    elem->data = data;

    if(lst->tail == NULL) {
        lst->head = elem;
        lst->crnt = elem;
    }
    else
        lst->tail->next = elem;

    lst->tail = elem;
    lst->changed = true;
    lst->length ++;

}

void prepend_llist(LList ll, void* data) {

    assert(ll != NULL);
    assert(data != NULL);
    llist* lst = (llist*)ll;

    llist_elem* elem = _ALLOC_T(llist_elem);
    elem->data = data;

    if(lst->head == NULL) {
        lst->tail = elem;
        lst->crnt = elem;
    }

    elem->next = lst->head;
    lst->head = elem;
    
    lst->changed = true;
    lst->length ++;
}

void* init_llist_iter(LList ll) {

    assert(ll != NULL);
    llist* lst = (llist*)ll;
    assert(lst->head != NULL);

    lst->crnt = lst->head;
    lst->changed = false;

    return lst->crnt->data;
}

void* iter_llist(LList ll) {

    assert(ll != NULL);
    llist* lst = (llist*)ll;
    void* data = NULL;

    if(lst->changed) 
        RAISE(LIST_ERROR, "list changed while iterating");

    if(lst->crnt != NULL) {
        data = lst->crnt->data;
        lst->crnt = lst->crnt->next;
    }

    return data;
}

void push_llist(LList ll, void* data) {

    assert(ll != NULL);
    assert(data != NULL);
    ENTER;
    prepend_llist(ll, data);
    RET;
}

void* pop_llist(LList ll) {

    assert(ll != NULL);
    llist* lst = (llist*)ll;
    void* data = NULL;

    if(lst->head != NULL) {
        data = lst->head->data;
        lst->head = lst->head->next;
    }
    else {
        lst->crnt = NULL;
        lst->tail = NULL;
    }

    lst->length --;
    return data;
}

void* peek_llist(LList ll) {

    assert(ll != NULL);
    llist* lst = (llist*)ll;

    void* data = NULL;

    if(lst->head != NULL) {
        data = lst->head->data;
    }

    return data;
}

int len_llist(LList ll) {

    assert(ll != NULL);
    llist* lst = (llist*)ll;

    return lst->length;
}
