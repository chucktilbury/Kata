/**
 * @file queue_test.c
 *
 * @brief Test the token queue. This test implements a small subset parser
 * that is contrived to verify that the token queue is working correctly.
 * The crux of the queue is that tokens can be marked as having been used
 * and the protocol for discarding tokens is working correctly. This test
 * also serves as a test bed to try out parser implementation ideas.
 *
 * Grammar:
 *
 *  module
 *      : (module_element)+
 *  module_element
 *      : scope_operator
 *      | namespace_definition
 *      | SYMBOL
 *  scope_operator
 *      : PUBLIC
 *      | PRIVATE
 *      | PROTECTED
 *  namespace_definition
 *      : NAMESPACE (SYMBOL)? namespace_body
 *  namespace_body
 *      : OCBRACE (module_element)* CCBRACE
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 01-12-2024
 * @copyright Copyright (c) 2024
 */
#include "ast.h"
#include "errors.h"
#include "scanner.h"
#include "trace.h"
#include "util.h"

extern void print_token(Token* tok);

// forward declarations
void* module_element();

/**
 * @brief Parse for a single keyword.
 *
 *  # A scope operator is a single keyword.
 *  scope_operator
 *      : PUBLIC
 *      | PRIVATE
 *      | PROTECTED
 */
void* scope_operator() {

    ENTER;
    AstNode* node = NULL;
    Token* tok    = get_token(); // this returns a pointer to the current token
    // void* post = post_token_queue(); // mark the position in the token queue

    if(tok->type == TOK_PUBLIC || tok->type == TOK_PRIVATE || tok->type == TOK_PROTECTED) {

        TRACE_TERM(tok);
        node = create_ast_node(AST_scope_operator);
        add_ast_attrib(node, "token", tok, sizeof(Token));
        finalize_token(); // mark this token as used
        advance_token();  // consume the token in the queue
    }
    // No tokens have been consumed so there is no need to reset the token
    // queue.

    RETV(node);
}

/**
 * @brief This is a recursive rule that parses for zero or more
 * module_elements.
 *
 *  # Zero or more module elements enclosed in braces.
 *  namespace_element_list
 *      : OCBRACE (module_element)* CCBRACE
 *
 */
void* namespace_element_list() {

    ENTER;
    AstNode* node = NULL;
    Token* tok    = get_token();
    // There are no cases where this can return without a valid parse
    // or an error.
    // void* post = post_token_queue();

    if(tok->type != TOK_OCBRACE) {
        show_syntax_error("expected a '{' but got %s", tok_to_str(tok->type));
        RETV(NULL);
    }
    else {
        TRACE("parsed a '{'");
        finalize_token();
        advance_token();
    }

    // get the module element list
    List* lst = create_list(sizeof(AstNode));
    while(true) {
        AstNode* me_node = NULL;

        if(NULL != (me_node = module_element())) {
            TRACE("adding a module element to the list");
            append_list(lst, me_node);
        }
        else if(get_token()->type == TOK_CCBRACE) {
            TRACE("parsed a '}'");
            node = create_ast_node(AST_namespace_element_list);
            add_ast_attrib(node, "list", lst, sizeof(List));
            TRACE("list size: %d", length_list(lst));
            finalize_token();
            advance_token();
            break;
        }
        else {
            // not a module_element or a CCBRACE, so it' an error
            show_syntax_error("expected a module element or a '}' but got "
                              "a %s",
                              tok_to_str(get_token()->type));
            RETV(NULL);
        }
    }

    finalize_token_queue(); // this is a valid nterm parse.
    RETV(node);
}

/**
 * @brief A namespace defintion has a keyword, an optional name, and a
 * body. Note that this is contrived for the test. A real namespace
 * requires the name.
 *
 *  # Keyword with an optional name and a body
 *  namespace_definition
 *      : NAMESPACE (SYMBOL)? namespace_element_list
 */
void* namespace_definition() {

    ENTER;
    AstNode* node = NULL;
    Token* tok    = get_token();
    // There are no cases where this can return and a token has been read
    // but there was no error. So there is no need to post the queue.
    // void* post = post_token_queue();

    if(tok->type == TOK_NAMESPACE) {
        // consume the token...
        finalize_token();
        tok = advance_token();
        TRACE("parsing a namespace");

        if(tok->type == TOK_SYMBOL) {
            // consume the token...
            TRACE_TERM(tok);
            node = create_ast_node(AST_namespace_definition);
            add_ast_attrib(node, "token", tok, sizeof(Token));
            finalize_token();
            advance_token(); // consume the token

            AstNode* nsel;
            if(NULL != (nsel = namespace_element_list())) {
                TRACE("have a valid namespace element list");
                add_ast_attrib(node, "nterm", nsel, sizeof(AstNode));
            }
            else {
                // syntax error, could not parse the list
                show_syntax_error("expected a namespace element");
                RETV(NULL);
            }
        }
        else {
            // syntax error, symbol required after namespace keyword
            show_syntax_error("expected SYMBOL token but got %s", tok_to_str(tok->type));
            RETV(NULL);
        }
    }

    RETV(node);
}

/**
 * @brief Parse for a single module element.
 *
 *  # Exactly one of these alternatives
 *  module_element
 *      : scope_operator
 *      | namespace_definition
 *      | SYMBOL
 */
void* module_element() {

    ENTER;
    AstNode* node = NULL;
    AstNode* mod_elem;
    void* post = post_token_queue();

    if(NULL != (mod_elem = scope_operator())) {
        TRACE("found a scope operator");
        node = create_ast_node(AST_module_element);
        add_ast_attrib(node, "nterm", mod_elem, sizeof(AstNode));
        finalize_token_queue();
    }
    else if(NULL != (mod_elem = namespace_definition())) {
        TRACE("found a namespace definition");
        node = create_ast_node(AST_module_element);
        add_ast_attrib(node, "nterm", mod_elem, sizeof(AstNode));
        finalize_token_queue();
    }
    else if(get_token()->type == TOK_SYMBOL) {
        TRACE_TERM(get_token());
        node = create_ast_node(AST_module_element);
        add_ast_attrib(node, "token", get_token(), sizeof(Token));
        // consume the token....
        finalize_token();
        advance_token();
        finalize_token_queue();
    }
    else {
        // else not an error
        reset_token_queue(post);
    }

    RETV(node);
}

/**
 * @brief Parse for a complete module.
 *
 *  # One or more module items.
 *  module
 *      : (module_item)+
 */
void* module() {

    ENTER;
    AstNode* mod_elem;
    List* node_lst = create_list(sizeof(AstNode));

    // one or more
    while(NULL != (mod_elem = module_element()))
        append_list(node_lst, (void*)mod_elem);

    Token* tok = get_token();
    if(tok->type != TOK_END_OF_FILE) {
        show_syntax_error("expected the end of file but got %s", tok_to_str(tok->type));
        RETV(NULL);
    }
    else {
        TRACE("handle end of file");
        close_file();
        // finalize_token(tok);
        tok = advance_token();
        // finalize_token_queue();
    }

    tok = get_token();
    if(tok->type != TOK_END_OF_INPUT) {
        show_syntax_error("expected the end of input but got %s", tok_to_str(tok->type));
        RETV(NULL);
    }
    TRACE("handle end of input and return node");

    AstNode* node = create_ast_node(AST_module);
    add_ast_attrib(node, "list", node_lst, sizeof(List));

    RETV(node);
}

#if 0
const char* type_to_str(AstType type) {

    return (type == AST_scope_operator)? "scope_operator" :
        (type == AST_type_name)? "type_name" :
        (type == AST_type_spec)? "type_spec" :
        (type == AST_type_spec_element)? "type_spec_element" :
        (type == AST_compound_name)? "compound_name" :
        (type == AST_namespace_element)? "namespace_element" :
        (type == AST_namespace_element_list)? "namespace_element_list" :
        (type == AST_namespace_definition)? "namespace_definition" :
        (type == AST_module_element)? "module_element" :
        (type == AST_module_element_list)? "module_element_list" :
        (type == AST_module)? "module" : "UNKNOWN";
}

int count = 0;

void traverse_ast(AstNode* tree) {

    ENTER;
    Token tok;
    AstNode node;
    AstType type;
    List list;

    get_ast_attrib(tree, "type", &type, sizeof(AstType));
    TRACE("type: \"%s\"", type_to_str(type));

    if(AST_OK == get_ast_attrib(tree, "token", &tok, sizeof(Token))) {
        TRACE_TERM(&tok);
    }

    if(AST_OK == get_ast_attrib(tree, "nterm", &node, sizeof(AstNode))) {
        traverse_ast(&node);
    }

    if(AST_OK == get_ast_attrib(tree, "list", &list, sizeof(List))) {
        TRACE("list length: %d", length_list(&list));
        ListIter* iter = init_list_iterator(&list);
        AstNode lnode;
        while(iterate_list(iter, &lnode)) {
            traverse_ast(&lnode);
        }
    }

    RET;
}
#endif

int main(int argc, char** argv) {

    ENTER;
    if(argc < 2) {
        fprintf(stderr, "Error: Need file name\n");
        RETV(1);
    }

    init_scanner();
    // runs from the build directory.
    open_file(argv[1]);

    // since the module is a list, this is only called once.
    printf("\nParse the input\n");
    AstNode* node = module();
    printf("End Parse\n");

    printf("\nDump the AST\n");
    traverse_ast(node);
    printf("End Dump\n\n");

    RETV(0);
}
