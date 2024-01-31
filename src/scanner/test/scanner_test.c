
#include "util.h"
#include "scanner.h"

extern void print_token(Token* tok);
extern Token* scan_token();
extern Token token;

void scan_file() {

    while(1) {
        print_token(&token);
        scan_token();
        if(token.type == TOK_END_OF_FILE) {
            print_token(&token);
            break;
        }
    }
}

int main(int argc, char** argv) {

    if(argc < 2) {
        fprintf(stderr, "Error: Need file name\n");
        return 1;
    }

    init_scanner();
    // runs from the build directory.
    open_file(argv[1]);

    int count = 1;
    while(1) {
        printf("%d. ", count++);
        if(token.type == TOK_IMPORT) {
            print_token(&token);
            scan_token(); // must be a string
            if(token.type == TOK_LITERAL_STR) {
                printf("%d. ", count++);
                print_token(&token);
                open_file(raw_string(token.str));
                printf("\n");
            }
        }
        else if(token.type == TOK_END_OF_FILE) {
            print_token(&token);
            printf("\n");
            close_file();
        }
        else if(token.type == TOK_END_OF_INPUT) {
            print_token(&token);
            break;
        }
        else
            print_token(&token);

        scan_token();
    }

    printf("\nfinished\n");
    return 0;
}