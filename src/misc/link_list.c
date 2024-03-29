/**
 * @file linked_list.c
 *
 * @brief Linked list functionality with reentrant iterator and stack.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 03-28-2024
 * @copyright Copyright (c) 2024
 */
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "memory.h"
#include "link_list.h"
#include "errors.h"

/**
 * @brief Create a link list object
 *
 * @return LinkList*
 *
 */
LinkList* create_link_list() {

    LinkList* lst = _ALLOC_T(LinkList);
    lst->first = NULL;
    lst->last = NULL;
    lst->is_changed = true;
    lst->length = 0;

    return lst;
}

/**
 * @brief Append an arbitrary pointer to the end of the list.
 *
 * @param lst
 * @param data
 *
 */
void append_link_list(LinkList* lst, void* data) {

    assert(lst != NULL);
    assert(data != NULL);

    LinkListElem* elem = _ALLOC_T(LinkListElem);
    elem->data = data;

    if(lst->last == NULL)
        lst->first = elem;
    else
        lst->last->next = elem;

    lst->last = elem;
    lst->is_changed = true;
    lst->length ++;

}

/**
 * @brief Insert an arbitrary pointer at the beginning of the list.
 *
 * @param lst
 * @param data
 *
 */
void prepend_link_list(LinkList* lst, void* data) {

    assert(lst != NULL);
    assert(data != NULL);

    LinkListElem* elem = _ALLOC_T(LinkListElem);
    elem->data = data;

    if(lst->first == NULL)
        lst->last = elem;
    elem->next = lst->first;
    lst->first = elem;

    lst->is_changed = true;
    lst->length ++;
}

/**
 * @brief Iterate the list from the beginning. The list cannot be modified
 * while it is being iterated. The iteration function is reentrant. The first
 * time the function is called, the post parameter should point to (void*)NULL.
 * After that, it should not be used for any purpose while the iteration is
 * happening. When the list reaches the end, then NULL is returned.
 *
 * @param lst
 * @param post
 * @return void*
 *
 */
void* iter_link_list(LinkList* lst, void** post) {

    assert(lst != NULL);

    LinkListElem* crnt = (LinkListElem*)(*post);

    if(crnt == NULL) {
        crnt = lst->first;
        lst->is_changed = false;
        *post = crnt;
        if(crnt != NULL)
            return crnt->data;
    }
    else if(lst->is_changed)
        fatal_error("list changed while iterating");
    else {
        if(crnt != (LinkListElem*)0x01) {
            crnt = crnt->next;
            if(crnt != NULL) {
                *post = crnt;
                return crnt->data;
            }
            else
                *post = (void*)0x01;
        }
        else
            return NULL;
    }


    return NULL;
}

/**
 * @brief Prepend an arbitrary pointer to the list.
 *
 * @param lst
 * @param data
 *
 */
void push_link_list(LinkList* lst, void* data) {

    assert(lst != NULL);
    assert(data != NULL);

    prepend_link_list(lst, data);
}

/**
 * @brief Remove the first pointer and return its value. Calling this on an
 * empty list returns NULL;
 *
 * @param lst
 * @return void*
 *
 */
void* pop_link_list(LinkList* lst) {

    assert(lst != NULL);
    void* data = NULL;

    if(lst->first != NULL) {
        data = lst->first->data;
        lst->first = lst->first->next;
    }
    else {
        lst->last = NULL;
    }

    lst->length --;
    return data;
}

/**
 * @brief Return the first value that is stored on the list. If the list is
 * empty, then NULL is returned.
 *
 * @param lst
 * @return void*
 *
 */
void* peek_link_list(LinkList* lst) {

    assert(lst != NULL);

    void* data = NULL;

    if(lst->first != NULL) {
        data = lst->first->data;
    }

    return data;
}

/**
 * @brief Return the last value that was stored on the list. If the list is
 * empty, then return NULL.
 *
 * @param lst
 * @return void*
 *
 */
void* last_link_list(LinkList* lst) {

    assert(lst != NULL);

    void* data = NULL;

    if(lst->last != NULL) {
        data = lst->last->data;
    }

    return data;
}

/**
 * @brief Return the number of list elements that are currently in the list.
 *
 * @param lst
 * @return int
 *
 */
int len_link_list(LinkList* lst) {

    assert(lst != NULL);

    return lst->length;
}

/**
 * @brief Sanity check.
 *
 * Build string
gcc -Wall -Wextra -g -DTEST_LINK_LIST \
     -I../scanner -I../util -I../../build/src/scanner \
     link_list.c memory.c errors.c \
     -L../../bin -lutil -lscan -lmisc -lutil
 */
#ifdef TEST_LINK_LIST

void print_list(LinkList* lst) {

    void* post = NULL;
    const char* str;

    printf("\n------------\n");
    //for(str = iter_link_list(lst, &post); str != NULL; str = iter_link_list(lst, &post))
    LLFOR(lst, str, post)
        printf("string: %s\n", str);
}

int main() {

    LinkList* lst = create_link_list();

    append_link_list(lst, "this is the first one");
    append_link_list(lst, "this is the second one");
    append_link_list(lst, "this is the third one");

    print_list(lst);

    push_link_list(lst, "push me");

    print_list(lst);

    const char* s = peek_link_list(lst);
    printf("\n----------\npeeked: %s\n", s);

    s = pop_link_list(lst);
    printf("\n----------\npopped: %s\n", s);

    s = peek_link_list(lst);
    printf("\n----------\npeeked: %s\n", s);

    s = pop_link_list(lst);
    printf("\n----------\npopped: %s\n", s);

    print_list(lst);

    s = pop_link_list(lst);
    printf("\n----------\npopped: %s\n", s);
    s = pop_link_list(lst);
    printf("\n----------\npopped: %s\n", s);
    print_list(lst);

    return 0;
}

#endif

