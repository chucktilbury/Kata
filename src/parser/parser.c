/**
 * @file parser.c
 *
 * @brief This contains routines and data structures that are common to all
 * other routines in the parser.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-05-2024
 * @copyright Copyright (c) 2024
 */
#include "parser.h"
#include "errors.h"
#include "internal_parser.h"
#include "scanner.h"
#include "trace.h"
#include "util.h"

static List* name_stack = NULL;

void pop_namespace() {

    ENTER;
    pop_list(name_stack, NULL);

    RET;
}

void push_namespace(Str* str) {

    ENTER;
    push_list(name_stack, str);

    RET;
}

Str* peek_namespace() {

    ENTER;
    Str str;

    peek_list(name_stack, &str);

    RETV(copy_string(&str));
}

AstModule* parser() {


    ENTER;
    AstModule* node = NULL;

    if(NULL != (node = parse_module())) {
        close_file();
        advance_token();
    }
    else {
        // error already given by parse_module()
        RETV(NULL);
    }

    Token* tok = get_token();

    if(tok->type != TOK_END_OF_INPUT) {
        EXPECTED("end of input");
        RETV(NULL);
    }

    RETV(node);

    // ENTER;
    // name_stack = create_list(sizeof(Str));

    // init_scanner();
    // open_file(fname);

    // AstNode* node = (AstNode*)parse_module();

    // // Note that this is a deficiency with these macros. If you return a
    // // function instance, it will get called two times.
    // RETV(node);
}
