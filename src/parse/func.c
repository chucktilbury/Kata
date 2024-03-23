/**
 * @file func.c
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-26-2024
 * @copyright Copyright (c) 2024
 */
#define USE_TRACE 1
#include "util.h"
#include "parse.h"
#include "scanner.h"

/**
 * @brief
 *
 *  function_reference
 *      = compound_reference expression_list compound_name_list
 *
 * @return ast_function_reference*
 *
 */
ast_function_reference* parse_function_reference() {

    ENTER;
    ast_function_reference* node = NULL;
    ast_compound_reference* name;
    ast_expression_list* inp;
    ast_compound_name_list* outp;
    void* post = post_token_queue();

    int state = 0;
    bool finished = false;

    while(!finished) {
        switch(state) {
            case 0:
                // compound reference required or no error
                if(NULL != (name = parse_compound_reference()))
                    state = 1;
                else
                    state = 101;
                break;

            case 1:
                // expression list required or error
                if(NULL != (inp = parse_expression_list()))
                    state = 2;
                else {
                    EXPECTED("an optional list of expressions");
                    state = 102;
                }
                break;

            case 2:
                // compound_name list required or error
                if(NULL != (outp = parse_compound_name_list()))
                    state = 100;
                else {
                    EXPECTED("an optional list of names");
                    state = 102;
                }
                break;

            case 100:
                // finished parsing the object
                node = CREATE_AST_NODE(AST_function_reference, ast_function_reference);
                node->name = name;
                node->inp = inp;
                node->outp = outp;
                finalize_token_queue();
                finished = true;
                break;

            case 101:
                // object is no match
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                // error parsing the object
                node = NULL;
                finished = true;
                break;
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  create_reference
 *      = create_name expression_list
 *
 * @return ast_create_reference*
 *
 */
ast_create_reference* parse_create_reference() {

    ENTER;
    ast_create_reference* node = NULL;
    ast_create_name* name;
    ast_expression_list* inp;
    void* post = post_token_queue();

    int state = 0;
    bool finished = false;

    while(!finished) {
        switch(state) {
            case 0:
                // name or not a match
                if(NULL != (name == parse_create_name()))
                    state = 1;
                else
                    state = 101;
                break;

            case 1:
                // expression list or error
                if(NULL != (inp = parse_expression_list()))
                    state = 100;
                else {
                    EXPECTED("an expression list");
                    state = 102;
                }
                break;

            case 100:
                // finished parsing
                node = CREATE_AST_NODE(AST_create_reference, ast_create_reference);
                node->name = name;
                node->inp = inp;
                finalize_token_queue();
                finished = true;
                break;

            case 101:
                // not a match
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                // error
                node = NULL;
                finished = true;
                break;
        }
    }

    RETV(node);
}


/**
 * @brief
 *
 *  destroy_reference
 *      = destroy_name
 *
 * @return ast_destroy_reference*
 *
 */
ast_destroy_reference* parse_destroy_reference() {

    ENTER;
    ast_destroy_reference* node = NULL;
    ast_destroy_name* name;
    void* post = post_token_queue();

    int state = 0;
    bool finished = false;

    while(!finished) {
        switch(state) {
            case 0:
                // destroy name or not a match
                if(NULL != (name == parse_create_name()))
                    state = 100;
                else
                    state = 101;
                break;

            case 100:
                // finished parsing
                node = CREATE_AST_NODE(AST_create_reference, ast_create_reference);
                node->name = name;
                finalize_token_queue();
                finished = true;
                break;

            case 101:
                // not a match
                reset_token_queue(post);
                finished = true;
                break;
        }
    }

    RETV(node);
}

/**
 * @brief The function qualifier has one,the other or both, but only one of
 * each. The function keyword is simply ignored.
 *
 *  func_qualifier
 *      = ('virtual')?
 *      / ('function')?
 *
 * @return ast_func_qualifier*
 *
 */
ast_func_qualifier* parse_func_qualifier() {

    ENTER;
    ast_func_qualifier* node = NULL;
    bool is_virtual = false;
    bool func_seen = false;

    int state = 0;
    bool finished = false;

    while(!finished) {
        switch(state) {
            case 0:
                // could be either keyword
                if(TOK_VIRTUAL == TTYPE) {
                    is_virtual = true;
                    advance_token();
                    state = 2;
                }
                else if(TOK_FUNCTION == TTYPE) {
                    advance_token();
                    func_seen = true;
                    state = 1;
                }
                else
                    state = 100;
                break;

            case 1:
                // could be the 'virtual' keyword or not a match
                if(TOK_VIRTUAL == TTYPE) {
                    is_virtual = true;
                    advance_token();
                    state = 100;
                }
                else
                    state = 100;
                break;

            case 2:
                // could be the 'function' keyword or not a match
                if(TOK_FUNCTION == TTYPE) {
                    advance_token();
                    func_seen = true;
                }
                state = 100;
                break;

            case 100:
                // create the data structure whether there is a match or not
                node = CREATE_AST_NODE(AST_func_qualifier, ast_func_qualifier);
                node->is_virtual = is_virtual;
                node->func_seen = func_seen;
                break;
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  function_declaration
 *      = (func_qualifier)? SYMBOL type_name_list type_name_list
 *
 * @return ast_function_declaration*
 *
 */
ast_function_declaration* parse_function_declaration() {

    ENTER;
    ast_function_declaration* node = NULL;
    Token* name;
    ast_func_qualifier* qual;
    ast_type_name_list* inputs;
    ast_type_name_list* outputs;
    void* post = post_token_queue();

    int state = 0;
    bool finished = false;

    while(!finished) {
        switch(state) {
            case 0:
                // qualifier is always a match.
                qual = parse_func_qualifier();
                state = 1;
                break;

            case 1:
                // SYMBOL token or not a match
                if(TOK_SYMBOL == TTYPE) {
                    name = get_token();
                    advance_token();
                    state = 2;
                }
                else if(qual->is_virtual || qual->func_seen) {
                    EXPECTED("a function definition");
                    state = 102;
                }
                else
                    state = 101;
                break;

            case 2:
                // type_name list or an error
                if(NULL != (inputs = parse_type_name_list()))
                    state = 3;
                else {
                    EXPECTED("a type name list");
                    state = 102;
                }
                break;

            case 3:
                // type_name_list or an error
                if(NULL != (outputs = parse_type_name_list()))
                    state = 3;
                else {
                    EXPECTED("a type name list");
                    state = 102;
                }
                break;

            case 100:
                // matching object
                finished = true;
                node = CREATE_AST_NODE(AST_function_declaration, ast_function_declaration);
                node->name = name;
                node->qual = qual;
                node->inputs = inputs;
                node->outputs = outputs;
                finalize_token_queue();
                break;

            case 101:
                // object not a match
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                // error
                node = NULL;
                finished = true;
                break;
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  create_declaration
 *      = (func_qualifier)? 'create' type_name_list
 *
 * @return ast_create_declaration*
 *
 */
ast_create_declaration* parse_create_declaration() {

    ENTER;
    ast_create_declaration* node = NULL;
    ast_func_qualifier* qual;
    ast_type_name_list* inputs;
    void* post = post_token_queue();

    int state = 0;
    bool finished = false;

    while(!finished) {
        switch(state) {
            case 0:
                // qualifier is always a match.
                qual = parse_func_qualifier();
                state = 1;
                break;

            case 1:
                // 'create' keyword or not a match
                if(TOK_CREATE == TTYPE)
                    state = 2;
                else if(qual->is_virtual || qual->func_seen) {
                    EXPECTED("a function definition");
                    state = 102;
                }
                else
                    state = 101;
                break;

            case 2:
                // type name list or error
                if(NULL != (inputs = parse_type_name_list()))
                    state = 100;
                else {
                    EXPECTED("a parameter list");
                    state = 102;
                }
                break;

            case 100:
                // matching object
                finished = true;
                node = CREATE_AST_NODE(AST_create_declaration, ast_create_declaration);
                node->qual = qual;
                node->inputs = inputs;
                finalize_token_queue();
                break;

            case 101:
                // object not a match
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                // error
                node = NULL;
                finished = true;
                break;
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  destroy_declaration
 *      = (func_qualifier)? 'destroy'
 *
 * @return ast_destroy_declaration*
 *
 */
ast_destroy_declaration* parse_destroy_declaration() {

    ENTER;
    ast_destroy_declaration* node = NULL;
    ast_func_qualifier* qual;
    void* post = post_token_queue();

    int state = 0;
    bool finished = false;

    while(!finished) {
        switch(state) {
            case 0:
                // qualifier is always a match.
                qual = parse_func_qualifier();
                state = 1;
                break;

            case 1:
                // 'destroy' keyword or not a match
                if(TOK_DESTROY == TTYPE)
                    state = 100;
                else if(qual->is_virtual || qual->func_seen) {
                    EXPECTED("a function definition");
                    state = 102;
                }
                else
                    state = 101;
                break;

            case 100:
                // matching object
                finished = true;
                node = CREATE_AST_NODE(AST_destroy_declaration, ast_destroy_declaration);
                node->qual = qual;
                finalize_token_queue();
                break;

            case 101:
                // object not a match
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                // error
                node = NULL;
                finished = true;
                break;
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  function_definition
 *      = (func_qualifier)? compound_name
 *          var_decl_list var_decl_list function_body
 *
 * @return ast_function_definition*
 *
 */
ast_function_definition* parse_function_definition() {

    ENTER;
    ast_function_definition* node = NULL;
    ast_func_qualifier* qual;
    ast_compound_reference* name;
    ast_var_decl_list* inputs;
    ast_var_decl_list* outputs;
    ast_function_body* body;
    void* post = post_token_queue();

    int state = 0;
    bool finished = false;

    while(!finished) {
        switch(state) {
            case 0:
                // qualifier is always a match.
                qual = parse_func_qualifier();
                state = 1;
                break;

            case 1:
                // compound reference or not a match
                break;

            case 2:
                // var decl list or not a match
                break;

            case 3:
                // var decl list or an error
                break;

            case 4:
                // function body or error
                break;

            case 100:
                // matching object
                finished = true;
                node = CREATE_AST_NODE(AST_function_definition, ast_function_definition);
                node->qual = qual;
                node->name = name;
                node->inputs = inputs;
                node->outputs = outputs;
                node->body = body;
                finalize_token_queue();
                break;

            case 101:
                // object not a match
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                // error
                node = NULL;
                finished = true;
                break;
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  create_name
 *      = SYMBOL ('.' SYMBOL)? '.' 'create'
 *
 * @return ast_create_name*
 *
 */
ast_create_name* parse_create_name() {

    ENTER;
    ast_create_name* node = NULL;
    LList list;
    void* post = post_token_queue();

    int state = 0;
    bool finished = false;

    while(!finished) {
        switch(state) {
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  destroy_name
 *      = SYMBOL ('.' SYMBOL)? '.' 'destroy'
 *
 * @return ast_destroy_name*
 *
 */
ast_destroy_name* parse_destroy_name() {

    ENTER;
    ast_destroy_name* node = NULL;
    LList list;
    void* post = post_token_queue();

    int state = 0;
    bool finished = false;

    while(!finished) {
        switch(state) {
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  create_definition
 *      = (func_qualifier)? create_name var_decl_list function_body
 *
 * @return ast_create_definition*
 *
 */
ast_create_definition* parse_create_definition() {

    ENTER;
    ast_create_definition* node = NULL;
    ast_create_name* name;
    ast_func_qualifier* qual;
    ast_var_decl_list* inputs;
    ast_function_body* body;
    void* post = post_token_queue();

    int state = 0;
    bool finished = false;

    while(!finished) {
        switch(state) {
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  destroy_definition
 *      = (func_qualifier)? destroy_name function_body
 *
 * @return ast_destroy_definition*
 *
 */
ast_destroy_definition* parse_destroy_definition() {

    ENTER;
    ast_destroy_definition* node = NULL;
    ast_destroy_name* name;
    ast_func_qualifier* qual;
    ast_var_decl_list* inputs;
    ast_function_body* body;
    void* post = post_token_queue();

    int state = 0;
    bool finished = false;

    while(!finished) {
        switch(state) {
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  function_body
 *      = '{' ( function_body_element )* '}'
 *
 * @return ast_function_body*
 *
 */
ast_function_body* parse_function_body() {

    ENTER;
    ast_function_body* node = NULL;
    LList list;
    void* post = post_token_queue();

    int state = 0;
    bool finished = false;

    while(!finished) {
        switch(state) {
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  start_function
 *      = ('function')? 'start' function_body
 *
 * @return ast_start_function*
 *
 */
ast_start_function* parse_start_function() {

    ENTER;
    ast_start_function* node = NULL;
    ast_function_body* body;
    void* post = post_token_queue();

    int state = 0;
    bool finished = false;

    while(!finished) {
        switch(state) {
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  function_assignment
 *      = compound_reference type_name_list type_name_list
 *
 * @return ast_function_assignment*
 *
 */
ast_function_assignment* parse_function_assignment() {


    ENTER;
    ast_function_assignment* node = NULL;
    ast_compound_reference* name;
    ast_type_name_list* inp;
    ast_type_name_list* outp;
    void* post = post_token_queue();

    int state = 0;
    bool finished = false;

    while(!finished) {
        switch(state) {
        }
    }

    RETV(node);
}

