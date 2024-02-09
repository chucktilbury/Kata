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
#include "util.h"
#include "scanner.h"
#include "internal_parser.h"
#include "parser.h"
#include "errors.h"
#include "trace.h"

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

AstNode* parser(const char* fname) {

    ENTER;
    name_stack = create_list(sizeof(Str));

    init_scanner();
    open_file(fname);

    AstNode* node = module();

    // Note that this is a deficiency with these macros. If you return a
    // function instance, it will get called two times.
    RETV(node);
}

