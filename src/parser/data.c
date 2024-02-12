/**
 * @file data.c
 *
 * @brief This file contains routines that parse for data declarations and
 * data definitions.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-10-2024
 * @copyright Copyright (c) 2024
 */
#include "internal_parser.h"

/**
 * @brief Single keyword that represents the name of a built-in type.
 *
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
AstTypeName* parse_type_name() {

    ENTER;
    Token* tok    = get_token();
    AstTypeName* node = NULL;
    void* post    = post_token_queue();

    if(tok->type == TOK_NUMBER ||
            tok->type == TOK_STRING ||
            tok->type == TOK_NOTHING ||
            tok->type == TOK_BOOLEAN ||
            tok->type == TOK_LIST ||
            tok->type == TOK_DICT) {

        TRACE_TERM(tok);
        node = CREATE_AST_NODE(AST_type_name, AstTypeName);
        //add_ast_attrib(node, "token", tok, sizeof(Token));
        node->tok = tok;
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
 * @brief A type spec element is a component of a complete type specification.
 *
 *      type_spec_element
 *          = type_name
 *          / compound_name
 *
 * @return NonTerm*
 *
 */
AstTypeSpecElement* parse_type_spec_element() {

    ENTER;
    AstNode *nterm;
    AstTypeSpecElement *node = NULL;
    void* post = post_token_queue();

    if((NULL != (nterm = (AstNode*)parse_type_name())) ||
            (NULL != (nterm = (AstNode*)parse_compound_name()))) {

        // the element is a valid type name
        TRACE_NTERM(nterm);
        node = CREATE_AST_NODE(AST_type_spec_element, AstTypeSpecElement);
        //add_ast_attrib(node, "nterm", nterm, sizeof(AstNode));
        node->elem = nterm;
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
 *
 *      type_spec
 *          : (CONST)? type_spec_element
 *
 * @return NonTerm*
 *
 */
AstTypeSpec* parse_type_spec() {

    ENTER;
    Token* tok = get_token();
    bool constant = false;
    void* post = post_token_queue();
    AstTypeSpec* node = NULL;

    if(tok->type == TOK_CONST) {
        TRACE_TERM(tok);
        constant = true;
        finalize_token();
        advance_token();
    }

    AstTypeSpecElement* nterm;
    if(NULL != (nterm = parse_type_spec_element())) {
        TRACE_NTERM((AstNode*)nterm);
        node = CREATE_AST_NODE(AST_type_spec, AstTypeSpec);
        //add_ast_attrib(node, "nterm", nterm, sizeof(AstNode));
        //add_ast_attrib(node, "is_const", &constant, sizeof(bool));
        node->elem = nterm;
        node->is_const = constant;
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
        show_syntax_error("expected a type name or a compound name but "
                          "got %s",
                          tok_to_str(tok->type));
    }

    RETV(node);
}
