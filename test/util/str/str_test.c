
#include "util.h"

int main() {

    Str* s = create_string("this is a test string");
    printf("%s\n", raw_string(s));

    add_string_str(s, " plus this one");
    printf("%s\n", raw_string(s));

    StrList* lst = create_str_list();
    add_str_list(lst, s);

    StrListIter* itr = init_str_list_iter(lst);
    Str* tpt;
    while(NULL != (tpt = iterate_str_list(itr)))
        printf("lst: %s\n", raw_string(tpt));


    return 0;
}