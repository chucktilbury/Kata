
#include <stdio.h>

#include "token.h"
#include "scanner.h"
#include "errors.h"
#include "parser.h"
#include "emit.h"


int main(int argc, char** argv) {

    LEVEL(10);
    ENTER;
    //int finished = 0;
    char* fname = NULL;

    if(argc >= 2)
        fname = argv[1];
    else
        fatal("must supply input file name");

    set_emitter(stdout);

    init_scanner();
    scanner_open(fname);

    parse();
    //token* tok;

    //do {

        //tok = scan_token();
        //fprintf(stdout, "Token: %d: %d: \"%s\" -> %s\n",
                    //get_line_no(), get_col_no(),
                    //get_string(tok->str), tok_to_str(tok->type));
        //if(tok->type == INPUT_END_TOK)
            //finished++;
        //destroy_token(tok);

    //} while(!finished);

    LEAVE;
}
