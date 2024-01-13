/**
 * @file queue_test.c
 *
 * @brief Test the token queue.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 01-12-2024
 * @copyright Copyright (c) 2024
 */
#include "util.h"
#include "scanner.h"

extern void print_token(Token* tok);

int main(int argc, char** argv) {

    if(argc < 2) {
        fprintf(stderr, "Error: Need file name\n");
        return 1;
    }

    init_scanner();
    // runs from the build directory.
    open_file(argv[1]);

//    Token* tok;

    printf("\nget same token\n");
    print_token(get_token());
    print_token(get_token());

    printf("\nadvance 3 tokens\n");
    print_token(advance_token());
    print_token(advance_token());
    print_token(advance_token());
    printf("\nprint same token\n");
    print_token(get_token());

    printf("\nreset token queue and print 3\n");
    reset_token_queue();
    print_token(advance_token());
    print_token(advance_token());
    print_token(advance_token());

    printf("\nconsume token queue and print 6\n");
    consume_token_queue();
    print_token(advance_token());
    print_token(advance_token());
    print_token(advance_token());
    print_token(advance_token());
    print_token(advance_token());
    print_token(advance_token());

    printf("\nrepeat token queue via iteration\n");
    void* mark = NULL;
    for(Token* tok = iterate_token_queue(&mark); tok != NULL; tok = iterate_token_queue(&mark))
        print_token(tok);


    return 0;
}