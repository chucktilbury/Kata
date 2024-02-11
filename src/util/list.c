
#include "util.h"

// The index is the item index, not the byte index. This function converts
// it to the byte index. If the idx is negative, then convert it to the
// correct byte index counting from the end.
static inline int normalize_index(List* lst, int idx) {

    int val;

    if(idx < 0)
        // if idx == -1, then result should be len.
        val = lst->len + (idx + 1);
    else
        val = idx;

    val *= lst->size;

    // TODO: raise exception instead of fatal error
    if(val < 0 || val > lst->len)
        RAISE(LIST_ERROR, "List Error: index out of range: %d\n", val);

    return val;
}

// There is no need to catch memory errors because these are intended to
// be fatal.
static inline void expand_buffer(List* lst) {

    if(lst->len + lst->size > lst->cap) {
        while(lst->len + lst->size > lst->cap)
            lst->cap <<= 1;
        lst->buffer = _REALLOC(lst->buffer, lst->size * lst->cap);
    }
}


List* create_list(int size) {

    List* ptr = _ALLOC_T(List);

    ptr->cap = 1 << 3;
    ptr->len = 0;
    ptr->size = size;
    ptr->buffer = _ALLOC(ptr->size * ptr->cap);
    ptr->changed = false;

    return ptr;
}

void destroy_list(List* lst) {

    if(lst != NULL) {
        _FREE(lst->buffer);
        _FREE(lst);
    }
}

// append a datum to the list
void append_list(List* lst, void* data) {

    expand_buffer(lst);

    memcpy(&lst->buffer[lst->len], data, lst->size);
    lst->len += lst->size;
    lst->changed = true;
}

void read_list(List* lst, int index, void* data) {

    int idx = normalize_index(lst, index);
    memcpy(data, &lst->buffer[idx], lst->size);
}

void write_list(List* lst, int index, void* data) {

    int idx = normalize_index(lst, index);
    memcpy(&lst->buffer[idx], data, lst->size);
}

void insert_list(List* lst, int index, void* data) {

    // aid in debugging
    int start, end, size;
    start = lst->size * normalize_index(lst, index);
    end = start + lst->size;
    size = lst->len - start;

    if((lst->size * index) < lst->len) {
        expand_buffer(lst);

        // make room
        memmove(&lst->buffer[end], &lst->buffer[start], size);

        // copy data to the location in the buffer
        memcpy(&lst->buffer[start], data, lst->size);
        lst->len += lst->size;
        lst->changed = true;
    }
    else
        RAISE(LIST_ERROR, "List Error: invalid index in insert list: %d", index);
}

void delete_list(List* lst, int index) {

    // aid in debugging
    int start, end, size;
    start = lst->size * normalize_index(lst, index);
    end = start + lst->size;
    size = lst->len - end;

    if(index >= 0 && ((lst->size * index) < lst->len)) {
        memmove(&lst->buffer[start], &lst->buffer[end], size);
        lst->len -= lst->size;
        lst->changed = true;
    }
    else
        RAISE(LIST_ERROR, "List Error: invalid index on delete list: %d", index);
}

void push_list(List* lst, void* data) {

    // printf("push: %p: %d (%d)\n", lst, lst->len, lst->size);
    append_list(lst, data);
}

void peek_list(List* lst, void* data) {

    if(lst->len >= lst->size)
        memcpy(data, &lst->buffer[lst->len - lst->size], lst->size);
    else
        RAISE(LIST_ERROR, "List Error: list is empty in peek list");
}

// places the NEW top of stack into the var.
void pop_list(List* lst, void* data) {
    // printf("pop: %p: %d (%d)\n", lst, lst->len, lst->size);
    if((lst->len - lst->size) >= 0)
        lst->len -= lst->size;
    else
        RAISE(LIST_ERROR, "List Error: list is empty in pop list");

    if(data != NULL)
        peek_list(lst, data);
}

void clear_list(List* lst) {

    lst->len = 0;
    lst->changed = true;
}

// Return the number of items in the list
int length_list(List* lst) {

    return lst->len / lst->size;
}

// Get the raw list
void* raw_list(List* lst) {

    return (void*)lst->buffer;
}

ListIter* init_list_iterator(List* lst) {

    ListIter* iter = _ALLOC_T(ListIter);
    iter->index = 0;
    iter->list = lst;
    lst->changed = false;

    return iter;
}

// If the list changes during iterations, then return an error.
int iterate_list(ListIter* iter, void* data) {

    if(!iter->list->changed) {
        if((iter->list->size * iter->index) < iter->list->len) {
            read_list(iter->list, iter->index++, data);
            return 1;
        }
        else
            return 0; // finished
    }
    else
        RAISE(LIST_ERROR, "List Error: list changed while iterating");
}

ListIter* init_list_riterator(List* lst) {

    ListIter* iter = _ALLOC_T(ListIter);
    iter->index = (lst->len / lst->size) - 1;
    iter->list = lst;
    lst->changed = false;

    return iter;
}

// If the list changes during iterations, then return an error.
int riterate_list(ListIter* iter, void* data) {

    if(!iter->list->changed) {
        if(iter->index >= 0) {
            read_list(iter->list, iter->index--, data);
            return 1;
        }
        else
            return 0; // ended
    }
    else
        RAISE(LIST_ERROR, "List Error: list changed while iterating");
}
