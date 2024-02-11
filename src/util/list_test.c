
#include "util.h"

void test_ints() {

    int count = 0;

    printf("tests on a list of integers\n");
    List* lst = create_base_list(sizeof(int));
    int value = 1000;

    printf("add 10 items\n");
    for(int i = 0; i < 10; i++) {
        add_base_list(lst, &value);
        value++;
        printf("add: len: %d cap: %d size: %d\n", lst->len, lst->cap, lst->size);
    }

    printf("iterate raw\n");
    int* l = raw_base_list(lst);
    for(int i = 0; i < 10; i++)
        printf("%d. list[%d]: %d\n", count++, i, l[i]);

    printf("selected gets\n");
    get_base_list(lst, 5, &value);
    printf("list[5]: %d\n", value);
    get_base_list(lst, 0, &value);
    printf("list[0]: %d\n", value);
    get_base_list(lst, 9, &value);
    printf("list[9]: %d\n", value);

    count = 0;
    printf("using the iterator\n");
    ListIter* iter = init_base_list_iter(lst);
    while(iter_base_list(iter, &value) == LIST_OK)
        printf("%d. value: %d\n", count++, value);

    count = 0;
    printf("using reverse the iterator\n");
    iter = init_base_list_riter(lst);
    while(riter_base_list(iter, &value) == LIST_OK)
        printf("%d. value: %d\n", count++, value);

    printf("remove item at index 5, 0, 0, 9\n");
    printf("pass: %d:\n", del_base_list(lst, 9));
    printf("del: len: %d cap: %d size: %d\n", lst->len, lst->cap, lst->size);
    del_base_list(lst, 5);
    printf("del: len: %d cap: %d size: %d\n", lst->len, lst->cap, lst->size);
    del_base_list(lst, 0);
    printf("del: len: %d cap: %d size: %d\n", lst->len, lst->cap, lst->size);
    printf("pass: %d:\n", del_base_list(lst, 0));
    printf("del: len: %d cap: %d size: %d\n", lst->len, lst->cap, lst->size);
    // this is an error and has no effect on the list
    printf("fail: %d:\n", del_base_list(lst, 9));
    printf("del: len: %d cap: %d size: %d\n", lst->len, lst->cap, lst->size);
    count = 0;
    iter = init_base_list_iter(lst);
    while(iter_base_list(iter, &value) == LIST_OK)
        printf("%d. value: %d\n", count++, value);

    printf("insert 8080 at 2\n");
    value = 8080;
    ins_base_list(lst, 2, &value);
    printf("ins: len: %d cap: %d size: %d\n", lst->len, lst->cap, lst->size);

    printf("insert 9090 at 2\n");
    value = 9090;
    ins_base_list(lst, 2, &value);
    printf("ins: len: %d cap: %d size: %d\n", lst->len, lst->cap, lst->size);

    printf("push 5050\n");
    value = 5050;
    push_base_list(lst, &value);
    printf("push: len: %d cap: %d size: %d\n", lst->len, lst->cap, lst->size);

    printf("insert 1111 at 0\n");
    value = 1111;
    ins_base_list(lst, 0, &value);
    printf("ins: len: %d cap: %d size: %d\n", lst->len, lst->cap, lst->size);

    iter = init_base_list_iter(lst);
    count = 0;
    while(iter_base_list(iter, &value) == LIST_OK)
        printf("%d. value: %d\n", count++, value);

    printf("insert 2222 at 9\n");
    value = 2222;
    ins_base_list(lst, 9, &value);
    printf("ins: len: %d cap: %d size: %d\n", lst->len, lst->cap, lst->size);

    printf("peek as stack\n");
    peek_base_list(lst, &value);
    printf("value: %d\n", value);
    printf("ins: len: %d cap: %d size: %d\n", lst->len, lst->cap, lst->size);

    iter = init_base_list_iter(lst);
    count = 0;
    while(iter_base_list(iter, &value) == LIST_OK)
        printf("%d. value: %d\n", count++, value);

    printf("pop value\n");
    pop_base_list(lst, &value);
    printf("value: %d\n", value);
    printf("ins: len: %d cap: %d size: %d\n", lst->len, lst->cap, lst->size);

    printf("pop value\n");
    pop_base_list(lst, &value);
    printf("value: %d\n", value);
    printf("ins: len: %d cap: %d size: %d\n", lst->len, lst->cap, lst->size);

    printf("pop value\n");
    pop_base_list(lst, &value);
    printf("value: %d\n", value);
    printf("ins: len: %d cap: %d size: %d\n", lst->len, lst->cap, lst->size);

    iter = init_base_list_iter(lst);
    count = 0;
    while(iter_base_list(iter, &value) == LIST_OK)
        printf("%d. value: %d\n", count++, value);
}

int main() {

    test_ints();

    return 0;
}