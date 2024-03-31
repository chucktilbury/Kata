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
#include "link_list.h"
#include "trace.h"
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
    LinkList* list = create_link_list();

    while(true) {
        if(NULL != (nterm = parse_module_item())) {
            append_link_list(list, nterm);
        }
        else if(TOK_END_OF_FILE == TTYPE) {
            node = CREATE_AST_NODE(AST_module, ast_module);
            node->list = list;
            finalize_token_queue();
            close_file();
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
 *      / function_definition
 *      / create_definition
 *      / destroy_definition
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
            (NULL != (nterm = (ast_node*)parse_function_definition())) ||
            (NULL != (nterm = (ast_node*)parse_create_definition())) ||
            (NULL != (nterm = (ast_node*)parse_destroy_definition())) ||
            (NULL != (nterm = (ast_node*)parse_var_definition()))) {

        node = CREATE_AST_NODE(AST_namespace_item, ast_namespace_item);
        node->nterm = nterm;
        node->scope = get_scope();
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

    if(TOK_NAMESPACE == TTYPE) {
        node = CREATE_AST_NODE(AST_namespace_definition, ast_namespace_definition);
        advance_token();
        node->scope = get_scope();
        push_scope(SCOPE_PRIV);

        if(TOK_SYMBOL == TTYPE) {
            node->name = get_token();
            TRACE_TERM(node->name);
            push_name(node->name->str);
            advance_token();

            if(TOK_OCBRACE == TTYPE) {
                advance_token();

                node->list = create_link_list();
                ast_node* nterm;
                while(true) {
                    if(NULL != (nterm = (ast_node*)parse_namespace_item()))
                        append_link_list(node->list, nterm);
                    else if(TOK_CCBRACE == TTYPE) {
                        TRACE("return here: %s", raw_string(get_compound_name()));
                        advance_token();
                        pop_scope();
                        pop_name();
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
            EXPECTED("+++ a SYMBOL");
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
 *      / class_var_declaration
 *      / function_declaration
 *      / create_declaration
 *      / destroy_declaration
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
            (NULL != (nterm = (ast_node*)parse_destroy_declaration())) ||
            (NULL != (nterm = (ast_node*)parse_create_declaration())) ||
            (NULL != (nterm = (ast_node*)parse_function_declaration())) ||
            (NULL != (nterm = (ast_node*)parse_class_var_declaration()))) {

        node = CREATE_AST_NODE(AST_module_item, ast_class_item);
        node->nterm = nterm;
        node->scope = get_scope();
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

    if(TOK_CLASS == TTYPE) {
        node = CREATE_AST_NODE(AST_class_definition, ast_class_definition);
        node->scope = get_scope();
        push_scope(SCOPE_PRIV);
        advance_token();

        if(TOK_SYMBOL == TTYPE) {
            node->name = get_token();
            push_name(node->name->str);
            advance_token();

            // optional type name
            if(TOK_OPAREN == TTYPE) {
                advance_token();

                ast_type_name* nterm;
                if(NULL != (nterm = parse_type_name()))
                    node->parent = nterm;
                else
                    node->parent = NULL;

                if(TOK_CPAREN == TTYPE) {
                    advance_token();
                }
                else {
                    EXPECTED("a ')'");
                    RETV(NULL);
                }
            }
            // else no parens

            if(TOK_OCBRACE == TTYPE) {
                advance_token();

                node->list = create_link_list();
                ast_node* nterm;
                while(true) {
                    if(NULL != (nterm = (ast_node*)parse_class_item()))
                        append_link_list(node->list, nterm);
                    else if(TOK_CCBRACE == TTYPE) {
                        pop_scope();
                        pop_name();
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


/**
 * @brief
 *
 *  class_var_declaration
 *      = ( 'var' / 'variable' ) var_decl
 *
 * @return ast_class_var_declaration*
 *
 */
ast_class_var_declaration* parse_class_var_declaration() {

    ENTER;
    ast_class_var_declaration* node = NULL;
    ast_var_decl* var;

    bool finished = false;
    int state = 0;
    void* post = post_token_queue();

    while(!finished) {
        switch(state) {
            case 0:
                // must be var or no match
                if(TOK_VAR == TTYPE || TOK_VARIABLE == TTYPE) {
                    advance_token();
                    state = 1;
                }
                else
                    state = 101;
                break;

            case 1:
                if(NULL != (var = parse_var_decl()))
                    state = 100;
                else {
                    EXPECTED("a variable declaration");
                    state = 102;
                }
                break;

            case 100:
                finished = true;
                node = CREATE_AST_NODE(AST_class_var_declaration, ast_class_var_declaration);
                node->var = var;
                finished = true;
                break;

            case 101:
                reset_token_queue(post);
                finished = true;
                break;

            case 102:
                finished = true;
                break;

            default:
                fatal_error("unexpected state in %s: %d", __func__, state);
        }
    }

    RETV(node);
}