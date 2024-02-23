/**
 * @file parser1.c
 *
 * @brief This is the bottom most level where the most simple objects are
 * parsed that are used many places.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 01-13-2024
 * @copyright Copyright (c) 2024
 */
#include "internal_parser.h"

/**
 * @brief This looks for a single keyword and returns the token associated
 * with it. This can only be used in conjunction with other rules, since it
 * can only return a terminal. Therefore, it cannot return an error.
 *      scope_operator
 *          = TOK_PUBLIC
 *          / TOK_PRIVATE
 *          / TOK_PROTECTED
 *
 * @return NonTerm*
 *
 */
AstScopeOperator* parse_scope_operator() {

    ENTER;
    Token* tok = get_token();
    AstScopeOperator* node = NULL;
    void* post = post_token_queue();

    if(tok->type == TOK_PUBLIC ||
            tok->type == TOK_PRIVATE ||
            tok->type == TOK_PROTECTED) {

        TRACE_TERM(tok);
        node = CREATE_AST_NODE(AST_scope_operator, AstScopeOperator);
        //node->tok = copy_token(tok);
        node->tok = tok;
        finalize_token();
        advance_token();
        finalize_token_queue();
    }
    else {
        TRACE("not a scope operator");
        reset_token_queue(post);
    }

    RETV(node);
}

/**
 * @brief Elements that can be in a node are similar to the elements that
 * be in a namespace.
 *      module_element
 *          = namespace_element
 *          / import_statement
 *
 * @return AstNode*
 *
 */
AstModuleElement* parse_module_element() {

    ENTER;
    AstNode *nterm;
    AstModuleElement* node = NULL;

    if((NULL != (nterm = (AstNode*)parse_namespace_element())) ||
            (NULL != (nterm = (AstNode*)parse_import_statement()))) {
        TRACE_NTERM(nterm);
        node = CREATE_AST_NODE(AST_module_element, AstModuleElement);
        node->elem = nterm;
        finalize_token_queue();
    }

    RETV(node);
}

/**
 * @brief Parse a compound name. A compound name has the format of
 *
 *      compound_name
 *          = SYMBOL ('.' SYMBOL)*
 *
 * @return NonTerm*
 *
 */
AstCompoundName* parse_compound_name() {

    ENTER;
    Token* tok;
    int state = 0;
    bool finished = false;

    AstCompoundName* nterm = NULL;
    PtrList* list = create_ptr_list();
    Str* strg = create_string(NULL);

    while(!finished) {
        switch(state) {
            case 0:
                tok = get_token();
                if(TOK_SYMBOL == tok->type) {
                    add_ptr_list(list, tok);
                    add_string_Str(strg, tok->str);
                    finalize_token();
                    advance_token();
                    state = 1;
                }
                else {
                    // exit with NULL, not an error
                    state = 101;
                }
                break;

            case 1:
                // could be a dot. if not then exit the state machine
                // with a valid nterm pointer.
                tok = get_token();
                if(TOK_DOT == tok->type) {
                    add_string_char(strg, '.');
                    finalize_token();
                    advance_token();
                    state = 2;
                }
                else {
                    // have a valid nterm
                    state = 100;
                }
                break;

            case 2:
                // must be a symbol. others are syntax error
                tok = get_token();
                if(TOK_SYMBOL == tok->type) {
                    add_ptr_list(list, tok);
                    add_string_Str(strg, tok->str);
                    finalize_token();
                    advance_token();
                    state = 1;
                }
                else {
                    // exit with error
                    EXPECTED("symbol after '.'");
                    state = 101;
                }
                break;

            case 100:
                nterm = CREATE_AST_NODE(AST_compound_name, AstCompoundName);
                nterm->lst = list;
                nterm->str = strg;
                finalize_token_queue();
                finished = true;
                break;

            case 101:
                // returning NULL (with GC)
                finished = true;
                break;

            default:
                fatal_error("invalid state in %s(): %d", __func__, state);
        }
    }

    RETV(nterm);
}

/**
 * @brief Top level entry point into the parser. Node returned is a
 * AST_module.
 *
 *      module = (module_elem)+
 *
 * @return AstNode*
 *
 */
AstModule* parse_module() {

    ENTER;
    AstNode *nterm;
    AstModule* node = NULL;
    PtrList* list = create_ptr_list();
    Token* tok;

    while(true) {
        if(NULL != (nterm = (AstNode*)parse_module_element())) {
            TRACE_NTERM(nterm);
            add_ptr_list(list, (void*)nterm);
        }
        else if(get_token()->type == TOK_END_OF_FILE) {
            // handle end of file
            TRACE_TERM(get_token());
            // pop_namespace();
            close_file();
            advance_token();
        }
        else
            break;
    }

    tok = get_token();
    if(tok->type == TOK_END_OF_INPUT) {
        TRACE_TERM(tok);
        node = CREATE_AST_NODE(AST_module, AstModule);
        //add_ast_attrib(node, "list", list, sizeof(List));
        node->lst = list;
        finalize_token_queue();
        RETV(node);
    }
    else {
        EXPECTED("end of input");
        //show_syntax_error("expected end of input but got %s", tok_to_str(tok->type));
        RETV(NULL);
    }
}
