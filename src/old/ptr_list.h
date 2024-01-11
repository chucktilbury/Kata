#ifndef PTR_LIST_H
#define PTR_LIST_H

typedef struct {
    void** list;
    int cap;
    int len;
} ptr_list_t;

ptr_list_t* create_ptr_list();
void destroy_ptr_list(ptr_list_t* lst);
void add_ptr_list(ptr_list_t* lst, void* data, size_t size);
void push_ptr_list(ptr_list_t* lst, void* data, size_t size);
void pop_ptr_list(ptr_list_t* lst);
void* peek_ptr_list(ptr_list_t* lst);

#endif
