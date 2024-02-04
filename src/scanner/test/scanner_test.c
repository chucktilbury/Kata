
#include "util.h"
#include "scanner.h"

extern void print_token(Token* tok);
extern Token* scan_token();
extern Token* sneak_token();
extern Token token;

void scan_file() {

    Token* tok = sneak_token();

    while(1) {
        print_token(tok);
        tok = scan_token();
        if(tok->type == TOK_END_OF_FILE) {
            print_token(tok);
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

    Token* tok = sneak_token();
    int count = 1;
    while(1) {
        printf("%d. ", count++);
        if(tok->type == TOK_IMPORT) {
            print_token(tok);
            tok = scan_token(); // must be a string
            if(tok->type == TOK_LITERAL_STR) {
                printf("%d. ", count++);
                print_token(tok);
                open_file(raw_string(tok->str));
                printf("\n");
            }
        }
        else if(tok->type == TOK_END_OF_FILE) {
            print_token(tok);
            printf("\n");
            close_file();
        }
        else if(tok->type == TOK_END_OF_INPUT) {
            print_token(tok);
            break;
        }
        else
            print_token(tok);

        tok = scan_token();
    }

    printf("\nfinished\n");
    return 0;
}