
#include "common.h"



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

    //init_scanner();
    //scanner_open(fname);
    //scanner_open("tests/test6.txt");
    ast_t* root = init_parser(fname);

    parse(root);

    dump_tree(root);
    //token* tok;

    //do {

        //tok = get_token();
        //fprintf(stdout, "Token: %d: %d: \"%s\" -> %s\n",
                    //get_line_no(), get_col_no(),
                    //tok->str, tok_to_str(tok->type));
        //if(tok->type == INPUT_END_TOK)
            //finished++;
        //next_token();

    //} while(!finished);

    LEAVE;
}
