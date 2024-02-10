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
AstNode* scope_operator() {

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
 * @brief An element that can be in a namespace.
 *      namespace_element
 *          = namespace_definition
 *          / type_spec
 *          / scope_operator
 *
 * @return AstNode*
 *
 */
AstNode* namespace_element() {

    ENTER;
    AstNode *nterm, *node = NULL;

    if((NULL != (nterm = namespace_definition())) ||
       (NULL != (nterm = type_spec())) || (NULL != (nterm = scope_operator()))) {

        TRACE_NTERM(nterm);
        node = create_ast_node(AST_namespace_element);
        add_ast_attrib(node, "nterm", nterm, sizeof(AstNode));
        finalize_token_queue();
    }

    RETV(node);
}

/**
 * @brief List of namespace elements.
 *      namespace_elem_list = '{' (namespace_elem)* '}'
 *
 * @return AstNode*
 *
 */
AstNode* namespace_element_list() {

    ENTER;
    Token* tok    = get_token();
    AstNode* node = NULL;

    if(tok->type == TOK_OCBRACE) {
        TRACE_TERM(tok);
        List* list = create_list(sizeof(AstNode));
        finalize_token();
        advance_token();

        while(true) {
            AstNode* nterm;
            if(NULL != (nterm = namespace_element())) {
                TRACE_NTERM(nterm);
                append_list(list, nterm);
            }
            else if(get_token()->type == TOK_CCBRACE) {
                TRACE_TERM(get_token());
                finalize_token();
                node = create_ast_node(AST_namespace_element_list);
                add_ast_attrib(node, "list", list, sizeof(List));
                finalize_token_queue();
                advance_token();
                break;
            }
            else {
                // not a namespace element or a CCBRACE
                show_syntax_error("expected a namespace element or a '}' "
                                  "but "
                                  "got %s",
                                  tok_to_str(get_token()->type));
                break;
            }
        }
    }
    else {
        // namespace requires a '{', so this is an error
        show_syntax_error("expected a '{', but got a %s", raw_string(tok->str));
    }

    RETV(node);
}

/**
 * @brief A complete namespace specification.
 *      namespace_definition
 *          = NAMESPACE SYMBOL namespace_element_list
 *
 * @return AstNode*
 *
 */
AstNode* namespace_definition() {

    ENTER;
    AstNode *nterm, *node = NULL;
    Token* tok = get_token();

    if(tok->type == TOK_NAMESPACE) {
        TRACE_TERM(tok);
        finalize_token();
        tok = advance_token();

        if(tok->type == TOK_SYMBOL) {
            TRACE_TERM(tok);
            node = create_ast_node(AST_namespace_definition);
            add_ast_attrib(node, "token", tok, sizeof(Token));
            finalize_token();
            advance_token(); // next token should be '{'

            if(NULL != (nterm = namespace_element_list())) {
                TRACE_NTERM(node);
                add_ast_attrib(node, "nterm", nterm, sizeof(AstNode));
                finalize_token_queue();
            }
            // else error is reported by namespace_element_list().
        }
        else {
            // namespace requires a name, so this is an error
            show_syntax_error("namespace expects a name, but got a %s",
                              raw_string(tok->str));
        }
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
AstNode* module_element() {

    ENTER;
    AstNode *nterm, *node = NULL;

    if((NULL != (nterm = namespace_element())) || (NULL != (nterm = import_statement()))) {

        TRACE_NTERM(nterm);
        node = create_ast_node(AST_module_element);
        add_ast_attrib(node, "nterm", nterm, sizeof(AstNode));
        finalize_token_queue();
    }

    RETV(node);
}

/**
 * @brief Top level entry point into the parser. Node returned is a
 * AST_module. module = (module_elem)+
 *
 * @return AstNode*
 *
 */
AstNode* module() {

    ENTER;
    AstNode *nterm, *node = NULL;
    List* list = create_list(sizeof(AstNode));
    Token* tok;

    while(true) {
        if(NULL != (nterm = module_element())) {
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
