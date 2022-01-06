
#include <stdio.h>

#include "token.h"
#include "scanner.h"
#include "errors.h"

int main(int argc, char** argv) {

    int finished = 0;
    char* fname;

    if(argc >= 2)
        fname = argv[1];
    else
        fatal("must supply file name");

    init_scanner();
    scanner_open(fname);
    token* tok;

    do {

        tok = scan_token();
        fprintf(stdout, "Token: %d: %d: \"%s\" -> %s\n",
                    get_line_no(), get_col_no(),
                    get_string(tok->str), tok_to_str(tok->type));
        if(tok->type == INPUT_END_TOK)
            finished++;
        destroy_token(tok);

    } while(!finished);
}
