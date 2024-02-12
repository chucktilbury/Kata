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
AstNode* parse_scope_operator() {

    ENTER;
    Token* tok    = get_token();
    AstNode* node = NULL;
    void* post    = post_token_queue();

    if(tok->type == TOK_PUBLIC || tok->type == TOK_PRIVATE || tok->type == TOK_PROTECTED) {

        TRACE_TERM(tok);
        node = create_ast_node(AST_scope_operator);
        add_ast_attrib(node, "token", tok, sizeof(Token));
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
AstNode* parse_module_element() {

    ENTER;
    AstNode *nterm, *node = NULL;

    if((NULL != (nterm = parse_namespace_element())) || (NULL != (nterm = parse_import_statement()))) {

        TRACE_NTERM(nterm);
        node = create_ast_node(AST_module_element);
        add_ast_attrib(node, "nterm", nterm, sizeof(AstNode));
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
AstNode* parse_compound_name() {

    ENTER;
    Token* tok    = get_token();
    void* post    = post_token_queue();
    AstNode* node = NULL;
    List* lst;

    if(tok->type == TOK_SYMBOL) {
        TRACE_TERM(tok);
        lst = create_list(sizeof(Token));
        append_list(lst, tok);
        finalize_token();
        tok = advance_token();

        if(tok->type == TOK_DOT) {
            // more than one element
            while(true) {
                finalize_token();
                tok = advance_token();

                if(tok->type == TOK_SYMBOL) {
                    // another element to add
                    TRACE_TERM(tok);
                    append_list(lst, tok);
                }
                else {
                    // handle an error because a dot must be followed by
                    // a TOK_SYMBOL in this context.
                    show_syntax_error("expected a symbol but got a %s",
                                      raw_string(tok->str));
                    RETV(NULL);
                }

                finalize_token();
                tok = advance_token();
                if(tok->type != TOK_DOT) {
                    // non-terminal is complete
                    node = create_ast_node(AST_compound_name);
                    add_ast_attrib(node, "tlist", lst, sizeof(List));
                    finalize_token_queue();
                    break;
                }
                // else it is a dot, so continue
            }
        }
        else {
            // single symbol is a match
            TRACE("single symbol compound");
            node = create_ast_node(AST_compound_name);
            add_ast_attrib(node, "tlist", lst, sizeof(List));
            finalize_token_queue();
        }
    }
    else {
        // not a match
        TRACE("not a compound symbol");
        reset_token_queue(post);
    }

    TRACE("complete list has %d items", length_list(lst));
    RETV(node);
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
AstNode* parse_module() {

    ENTER;
    AstNode *nterm, *node = NULL;
    List* list = create_list(sizeof(AstNode));
    Token* tok;

    while(true) {
        if(NULL != (nterm = parse_module_element())) {
            TRACE_NTERM(nterm);
            append_list(list, (void*)nterm);
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
        node = create_ast_node(AST_module);
        add_ast_attrib(node, "list", list, sizeof(List));
        finalize_token_queue();
        RETV(node);
    }
    else {
        show_syntax_error("expected end of input but got %s", tok_to_str(tok->type));
        RETV(NULL);
    }
}
