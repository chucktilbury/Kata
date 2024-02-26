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
AstNamespaceElement* parse_namespace_element() {

    ENTER;
    AstNode *nterm;
    AstNamespaceElement* node = NULL;

    if((NULL != (nterm = (AstNode*)parse_namespace_definition())) ||
            (NULL != (nterm = (AstNode*)parse_type_spec())) ||
            (NULL != (nterm = (AstNode*)parse_scope_operator()))) {

        TRACE_NTERM(nterm);
        node = CREATE_AST_NODE(AST_namespace_element, AstNamespaceElement);
        //add_ast_attrib(node, "nterm", nterm, sizeof(AstNode));
        node->elem = nterm;
        finalize_token_queue();
    }

    RETV(node);
}

/**
 * @brief List of namespace elements.
 *      namespace_body = '{' (namespace_elem)* '}'
 *
 * @return AstNode*
 *
 */
AstNamespaceBody* parse_namespace_body() {

    ENTER;
    Token* tok = get_token();
    AstNamespaceBody* node = NULL;

    if(tok->type == TOK_OCBRACE) {
        TRACE_TERM(tok);
        PtrList* list = create_ptr_list(sizeof(AstNode));
        finalize_token();
        advance_token();

        while(true) {
            AstNode* nterm;
            if(NULL != (nterm = (AstNode*)parse_namespace_element())) {
                TRACE_NTERM(nterm);
                add_ptr_list(list, nterm);
            }
            else if(get_token()->type == TOK_CCBRACE) {
                TRACE_TERM(get_token());
                finalize_token();
                node = CREATE_AST_NODE(AST_namespace_body, AstNamespaceBody);
                //add_ast_attrib(node, "list", list, sizeof(List));
                node->lst = list;
                finalize_token_queue();
                advance_token();
                break;
            }
            else {
                // not a namespace element or a CCBRACE
                // show_syntax_error("expected a namespace element or a '}' "
                //                   "but "
                //                   "got %s",
                //                   tok_to_str(get_token()->type));
                EXPECTED("namespace element or a '}'");
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
 *          = 'namespace' SYMBOL namespace_body
 *
 * @return AstNode*
 *
 */
AstNamespaceDefinition* parse_namespace_definition() {

    ENTER;
    AstNamespaceBody *nterm;
    AstNamespaceDefinition *node = NULL;
    Token* tok = get_token();

    if(tok->type == TOK_NAMESPACE) {
        TRACE_TERM(tok);
        finalize_token();
        tok = advance_token();

        if(tok->type == TOK_SYMBOL) {
            TRACE_TERM(tok);
            node = CREATE_AST_NODE(AST_namespace_definition, AstNamespaceDefinition);
            //add_ast_attrib(node, "token", tok, sizeof(Token));
            node->name = tok;
            finalize_token();
            advance_token(); // next token should be '{'

            if(NULL != (nterm = parse_namespace_body())) {
                TRACE_NTERM((AstNode*)node);
                //add_ast_attrib(node, "nterm", nterm, sizeof(AstNode));
                node->body = nterm;
                finalize_token_queue();
            }
            // else error is reported by namespace_element_list().
        }
        else {
            // namespace requires a name, so this is an error
            //show_syntax_error("namespace expects a name, but got a %s",
            //                  raw_string(tok->str));
            EXPECTED("a name");
        }
    }

    RETV(node);
}
