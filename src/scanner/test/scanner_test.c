
#include "util.h"
#include "scanner.h"

extern void print_token(Token* tok);
extern Token* scan_token();
extern Token token;

void scan_file() {

    while(1) {
        print_token(&token);
        scan_token();
        if(token.type == END_OF_INPUT) {
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
        printf("%d. ", count);
        print_token(&token);
        scan_token();
        if(token.type == END_OF_INPUT) {
            print_token(&token);
            break;
        }
        else if(count++ == 5)
            open_file(argv[2]);
    }


    printf("stuff and junk\n");
    return 0;
}