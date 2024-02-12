/**
 * @file namespace.c
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-11-2024
 * @copyright Copyright (c) 2024
 */
#include "internal_parser.h"

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
AstNode* parse_namespace_element() {

    ENTER;
    AstNode *nterm, *node = NULL;

    if((NULL != (nterm = parse_namespace_definition())) ||
       (NULL != (nterm = parse_type_spec())) || (NULL != (nterm = parse_scope_operator()))) {

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
AstNode* parse_namespace_element_list() {

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
            if(NULL != (nterm = parse_namespace_element())) {
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
AstNode* parse_namespace_definition() {

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

            if(NULL != (nterm = parse_namespace_element_list())) {
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
