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
    Token* tok = get_token();
    AstNode* node = NULL;
    void* post = post_token_queue();

    if(tok->type == TOK_PUBLIC ||
            tok->type == TOK_PRIVATE ||
            tok->type == TOK_PROTECTED) {

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
 * @brief Single keyword that represents the name of a built-in type.
 *      type_name
 *          = TOK_NUMBER
 *          / TOK_NOTHING
 *          / TOK_STRING
 *          / TOK_BOOLEAN
 *          / TOK_LIST
 *          / TOK_DICT
 *
 * @return NonTerm*
 *
 */
AstNode* type_name() {

    ENTER;
    Token* tok = get_token();
    AstNode* node = NULL;
    void* post = post_token_queue();

    if(tok->type == TOK_NUMBER ||
            tok->type == TOK_STRING ||
            tok->type == TOK_NOTHING ||
            tok->type == TOK_BOOLEAN ||
            tok->type == TOK_LIST ||
            tok->type == TOK_DICT) {

        TRACE_TERM(tok);
        node = create_ast_node(AST_type_name);
        add_ast_attrib(node, "token", tok, sizeof(Token));
        finalize_token();
        advance_token();
        finalize_token_queue();
    }
    else {
        TRACE("not a type name");
        reset_token_queue(post);
    }

    RETV(node);
}

/**
 * @brief Parse a compound name. A compound name has the format of
 *      compound_name
 *          = SYMBOL ('.' SYMBOL)*
 *
 * @return NonTerm*
 *
 */
AstNode* compound_name() {

    ENTER;
    Token* tok = get_token();
    void* post = post_token_queue();
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
                    handle_error("expected a symbol but got a %s",
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
 * @brief A type spec element is a component of a complete type specification.
 *      type_spec_element
 *          = type_name
 *          / compound_name
 *
 * @return NonTerm*
 *
 */
AstNode* type_spec_element() {

    ENTER;
    AstNode* nterm, *node = NULL;
    void* post = post_token_queue();

    if((NULL != (nterm = type_name())) ||
            (NULL != (nterm = compound_name()))) {

        // the element is a valid type name
        TRACE_NTERM(nterm);
        node = create_ast_node(AST_type_spec_element);
        add_ast_attrib(node, "nterm", nterm, sizeof(AstNode));
        finalize_token_queue();
    }
    else {
        // this is not a match, but it could match the next alternative in
        // the caller.
        TRACE("not a type spec element");
        reset_token_queue(post);
    }

    RETV(node);
}

/**
 * @brief A type spec is a complete specification to name a type.
 *      type_spec
 *          : (CONST)? type_spec_element
 *
 * @return NonTerm*
 *
 */
AstNode* type_spec() {

    ENTER;
    Token* tok = get_token();
    bool constant = false;
    void* post = post_token_queue();
    AstNode* node = NULL;

    if(tok->type == TOK_CONST) {
        TRACE_TERM(tok);
        constant = true;
        finalize_token();
        advance_token();
    }

    AstNode* nterm;
    if(NULL != (nterm = type_spec_element())) {
        TRACE_NTERM(nterm);
        node = create_ast_node(AST_type_spec);
        add_ast_attrib(node, "nterm", nterm, sizeof(AstNode));
        add_ast_attrib(node, "is_const", &constant, sizeof(bool));
        finalize_token_queue();
    }
    // if there is no const and it's not a type_spec_element, then it's not
    // an error, just rewind the token queue and return NULL.
    else if(!constant) {
        TRACE("not a type_spec");
        reset_token_queue(post);
    }
    // If the const keyword was present, and it's not a type_spec_element,
    // then that is a syntax error.
    else {
        tok = get_token();
        handle_error("expected a type name or a compound name but got %s",
                        tok_to_str(tok->type));
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
    AstNode* nterm, *node = NULL;

    if((NULL != (nterm = namespace_definition())) ||
            (NULL != (nterm = type_spec())) ||
            (NULL != (nterm = scope_operator()))) {

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
    Token* tok = get_token();
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
                handle_error("expected a namespace element or a '}' but got %s", tok_to_str(get_token()->type));
                break;
            }
        }
    }
    else {
        // namespace requires a '{', so this is an error
        handle_error("expected a '{', but got a %s", raw_string(tok->str));
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
    AstNode* nterm, *node = NULL;
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
            handle_error("namespace expects a name, but got a %s",
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
    AstNode* nterm, *node = NULL;

    if((NULL != (nterm = namespace_element())) ||
            (NULL != (nterm = import_statement()))) {

        TRACE_NTERM(nterm);
        node = create_ast_node(AST_module_element);
        add_ast_attrib(node, "nterm", nterm, sizeof(AstNode));
        finalize_token_queue();
    }

    RETV(node);
}

/**
 * @brief Top level entry point into the parser. Node returned is a AST_module.
 *      module = (module_elem)+
 *
 * @return AstNode*
 *
 */
AstNode* module() {

    ENTER;
    AstNode* nterm, *node = NULL;
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
            //pop_namespace();
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
        handle_error("expected end of input but got %s",
                    tok_to_str(tok->type));
        RETV(NULL);
    }
}
