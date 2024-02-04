/**
 * @file queue_test.c
 *
 * @brief Test the token queue. This test implements a small subset parser
 * that is contrived to verify that the token queue is working correctly. The
 * crux of the queue is that tokens can be marked as having been used and the
 * protocol for discarding tokens is working correctly. This test also serves
 * as a test bed to try out parser implementation ideas.
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
#include "util.h"
#include "scanner.h"
#include "ast.h"
#include "errors.h"

#define USE_TRACE 1

#if USE_TRACE
static int count = 0;
static const int increment = 4;
#define ENTER do { \
        printf("%*senter: %s: %d\n", count, "", __func__, __LINE__); \
        count+=increment; \
    } while(false)

#define TRACE(f, ...) do { \
        printf("%*s%d: ", count, "", __LINE__); \
        printf(f, ##__VA_ARGS__); \
        printf("\n"); \
    } while(false)

#define RET(v) do { \
        count-=increment; \
        printf("%*sret: %s: %d: %p\n", count, "", __func__, __LINE__, ((void*)v)); \
        return (v); \
    } while(false)
#else
#define ENTER
#define TRACE(f, ...)
#define RET(v) return v;
#endif

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
    Token* tok = get_token(); // this returns a pointer to the current token
    //void* post = post_token_queue(); // mark the position in the token queue

    if(tok->type == TOK_PUBLIC ||
            tok->type == TOK_PRIVATE ||
            tok->type == TOK_PROTECTED) {

        TRACE("found a \"%s\"", tok_to_str(tok->type));
        node = create_ast_node(AST_scope_operator);
        add_ast_attrib(node, "payload", tok, sizeof(Token));
        finalize_token(tok); // mark this token as used
        advance_token(); // consume the token in the queue
    }
    // No tokens have been consumed so there is no need to reset the token
    // queue.

    RET(node);
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
    Token* tok = get_token();
    // There are no cases where this can return without a valid parse
    // or an error.
    //void* post = post_token_queue();

    if(tok->type != TOK_OCBRACE) {
        handle_error("expected a '{' but got %s", tok_to_str(tok->type));
        return NULL;
    }
    else {
        finalize_token(tok);
        advance_token();
    }

    // get the module element list
    while(true) {
        AstNode* me_node = NULL;
        List* lst = create_list(sizeof(AstNode));

        if(NULL != (me_node = module_element())) {
            append_list(lst, me_node);
        }
        else if(get_token()->type == TOK_CCBRACE) {
            node = create_ast_node(AST_namespace_element_list);
            add_ast_attrib(node, "payload", lst, sizeof(List));
            finalize_token(tok);
            advance_token();
            break;
        }
        else {
            // not a module_element or a CCBRACE, so it' an error
            handle_error("expected a module element or a '}' but got a %s", tok_to_str(get_token()->type));
            RET(NULL);
        }
    }

    finalize_token_queue(); // this is a valid nterm parse.
    RET(node);
}

/**
 * @brief A namespace defintion has a keyword, an optional name, and a body.
 * Note that this is contrived for the test. A real namespace requires the
 * name.
 *
 *  # Keyword with an optional name and a body
 *  namespace_definition
 *      : NAMESPACE (SYMBOL)? namespace_element_list
 */
void* namespace_definition() {

    ENTER;
    AstNode* node = NULL;
    Token* tok = get_token();
    // There are no cases where this can return and a token has been read
    // but there was no error. So there is no need to post the queue.
    //void* post = post_token_queue();

    if(tok->type == TOK_NAMESPACE) {
        // consume the token...
        finalize_token(tok);
        tok = advance_token();
        TRACE("parsing a namespace");

        if(tok->type == TOK_SYMBOL) {
            // consume the token...
            TRACE("have a name: \"%s\"", raw_string(tok->str));
            node = create_ast_node(AST_namespace_definition);
            add_ast_attrib(node, "name", tok, sizeof(Token));
            finalize_token(tok);
            advance_token(); // consume the token

            AstNode* nsel;
            if(NULL != (nsel = namespace_element_list())) {
                TRACE("have a valid namespace element list");
                add_ast_attrib(node, "payload", nsel, sizeof(AstNode));
            }
            else {
                // syntax error, could not parse the list
                handle_error("expected a namespace element");
                RET(NULL);
            }
        }
        else {
            // syntax error, symbol required after namespace keyword
            handle_error("expected SYMBOL token but got %s", tok_to_str(tok->type));
            RET(NULL);
        }
    }

    RET(node);
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
        add_ast_attrib(node, "payload", mod_elem, sizeof(AstNode));
        finalize_token_queue();
    }
    else if(NULL != (mod_elem = namespace_definition())) {
        TRACE("found a namespace definition");
        node = create_ast_node(AST_module_element);
        add_ast_attrib(node, "payload", mod_elem, sizeof(AstNode));
        finalize_token_queue();
    }
    else if(get_token()->type == TOK_SYMBOL) {
        TRACE("found a symbol definition: \"%s\"", raw_string(get_token()->str));
        node = create_ast_node(AST_module_element);
        add_ast_attrib(node, "payload", get_token(), sizeof(Token));
        // consume the token....
        finalize_token(get_token());
        advance_token();
        finalize_token_queue();
    }
    else {
        // else not an error
        reset_token_queue(post);
    }

    RET(node);
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
        handle_error("expected the end of file but got %s", tok_to_str(tok->type));
        RET(NULL);
    }
    else {
        TRACE("handle end of file");
        close_file();
        //finalize_token(tok);
        tok = advance_token();
        //finalize_token_queue();
    }

    tok = get_token();
    if(tok->type != TOK_END_OF_INPUT) {
        handle_error("expected the end of input but got %s", tok_to_str(tok->type));
        RET(NULL);
    }
    TRACE("handle end of input and return node");

    AstNode* node = create_ast_node(AST_module);
    add_ast_attrib(node, "payload", node_lst, sizeof(List));

    RET(node);
}

int main(int argc, char** argv) {

    ENTER;
    if(argc < 2) {
        fprintf(stderr, "Error: Need file name\n");
        return 1;
    }

    init_scanner();
    // runs from the build directory.
    open_file(argv[1]);

    // since the module is a list, this is only called once.
    module();

    RET(0);
}