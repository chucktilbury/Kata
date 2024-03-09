/**
 * @file module.c
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
 *  module
 *      = ( module_item )+
 *
 * @return ast_module*
 *
 */
ast_module* parse_module() {

    ENTER;
    ast_module* node = NULL;
    ast_module_item* nterm;
    LList list = create_llist();

    while(true) {
        if(NULL != (nterm = parse_module_item())) {
            append_llist(list, nterm);
        }
        else if(TOK_END_OF_FILE == TOK_TYPE) {
            node = CREATE_AST_NODE(AST_module, ast_module);
            node->list = list;
            finalize_token_queue();
            close_file();
            advance_token();
            TRACE("end of file");
            break;
        }
        else {
            EXPECTED("module item or end of file");
            node = NULL;
            break;
        }
    }

    RETV(node);
}

/**
 * @brief
 *
 *  module_item
 *      = namespace_item
 *      / import_statement
 *      / start_function
 *
 * @return ast_module_item*
 *
 */
ast_module_item* parse_module_item() {

    ENTER;
    ast_module_item* node = NULL;
    ast_node* nterm;
    void* post = post_token_queue();

    if((NULL != (nterm = (ast_node*)parse_namespace_item())) ||
            (NULL != (nterm = (ast_node*)parse_import_statement())) ||
            (NULL != (nterm = (ast_node*)parse_start_function()))) {

        node = CREATE_AST_NODE(AST_module_item, ast_module_item);
        node->nterm = nterm;
        node->scope = get_scope();
        finalize_token_queue();
    }
    else {
        // returning NULL, but no error.... yet...
        reset_token_queue(post);
    }

    RETV(node);
}

/**
 * @brief
 *
 *  namespace_item
 *      = scope_operator
 *      / namespace_definition
 *      / class_definition
 *      / func_definition
 *      / var_definition
 *
 * @return ast_namespace_item*
 *
 */
ast_namespace_item* parse_namespace_item() {

    ENTER;
    ast_namespace_item* node = NULL;
    ast_node* nterm;
    void* post = post_token_queue();

    if((NULL != (nterm = (ast_node*)parse_scope_operator())) ||
            (NULL != (nterm = (ast_node*)parse_namespace_definition())) ||
            (NULL != (nterm = (ast_node*)parse_class_definition())) ||
            (NULL != (nterm = (ast_node*)parse_func_definition())) ||
            (NULL != (nterm = (ast_node*)parse_var_definition()))) {

        node = CREATE_AST_NODE(AST_namespace_item, ast_namespace_item);
        node->nterm = nterm;
        node->scope = get_scope();
        finalize_token_queue();
    }
    else {
        // returning NULL, but no error.... yet...
        reset_token_queue(post);
    }

    RETV(node);
}

/**
 * @brief
 *
 *  namespace_definition
 *      = 'namespace' SYMBOL '{' ( namespace_item )+ '}'
 *
 * @return ast_namespace_definition*
 *
 */
ast_namespace_definition* parse_namespace_definition() {

    ENTER;
    ast_namespace_definition* node = NULL;

    if(TOK_NAMESPACE == TOK_TYPE) {
        node = CREATE_AST_NODE(AST_namespace_definition, ast_namespace_definition);
        node->scope = get_scope();
        advance_token();

        if(TOK_SYMBOL == TOK_TYPE) {
            node->name = get_token();
            advance_token();

            if(TOK_OCBRACE == TOK_TYPE) {
                advance_token();

                node->list = create_llist();
                ast_node* nterm;
                while(true) {
                    if(NULL != (nterm = (ast_node*)parse_namespace_item()))
                        append_llist(node->list, nterm);
                    else if(TOK_CCBRACE == TOK_TYPE) {
                        advance_token();
                        break;
                    }
                    else {
                        EXPECTED("namespace item or a '}'");
                        node = NULL;
                        break;
                    }
                }
            }
            else {
                EXPECTED("a '{'");
                node = NULL;
            }
        }
        else {
            EXPECTED("a SYMBOL");
            node = NULL;
        }
    }
    // else not a namespace definition

    RETV(node);
}

/**
 * @brief
 *
 *  class_item
 *      = scope_operator
 *      / var_decl
 *      / func_decl
 *
 * @return ast_class_item*
 *
 */
ast_class_item* parse_class_item() {

    ENTER;
    ast_class_item* node = NULL;
    ast_node* nterm;
    void* post = post_token_queue();

    if((NULL != (nterm = (ast_node*)parse_scope_operator())) ||
            (NULL != (nterm = (ast_node*)parse_var_decl())) ||
            (NULL != (nterm = (ast_node*)parse_func_decl()))) {

        node = CREATE_AST_NODE(AST_module_item, ast_class_item);
        node->nterm = nterm;
        node->scope = get_scope();
        TRACE_TERM(get_token());
        finalize_token_queue();
        TRACE_TERM(get_token());
    }
    else {
        // returning NULL, but no error.... yet...
        TRACE_TERM(get_token());
        reset_token_queue(post);
    }

    RETV(node);
}

/**
 * @brief
 *
 *  class_definition
 *      = 'class' SYMBOL ( '(' ( type_name )? ')' )?
 *              '{' ( class_item )+ '}'
 *
 * @return ast_class_definition*
 *
 */
ast_class_definition* parse_class_definition() {

    ENTER;
    ast_class_definition* node = NULL;

    if(TOK_CLASS == TOK_TYPE) {
        node = CREATE_AST_NODE(AST_class_definition, ast_class_definition);
        node->scope = get_scope();
        advance_token();

        if(TOK_SYMBOL == TOK_TYPE) {
            node->name = get_token();
            advance_token();

            // optional type name
            if(TOK_OPAREN == TOK_TYPE) {
                advance_token();

                ast_type_name* nterm;
                if(NULL != (nterm = parse_type_name()))
                    node->parent = nterm;
                else
                    node->parent = NULL;

                if(TOK_CPAREN == TOK_TYPE) {
                    advance_token();
                }
                else {
                    EXPECTED("a ')'");
                    RETV(NULL);
                }
            }
            // else no parens

            if(TOK_OCBRACE == TOK_TYPE) {
                advance_token();

                node->list = create_llist();
                ast_node* nterm;
                while(true) {
                    if(NULL != (nterm = (ast_node*)parse_class_item()))
                        append_llist(node->list, nterm);
                    else if(TOK_CCBRACE == TOK_TYPE) {
                        advance_token();
                        break;
                    }
                    else {
                        EXPECTED("class item or a '}'");
                        node = NULL;
                        break;
                    }
                }
            }
            else {
                EXPECTED("a '{'");
                node = NULL;
            }
        }
        else {
            EXPECTED("a SYMBOL");
            node = NULL;
        }

    }
    // else not a class definition

    RETV(node);
}


