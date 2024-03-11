/**
 * @file func_body.c
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
 *  function_body_element
 *      = var_definition
 *      / function_reference
 *      / create_reference
 *      / destroy_reference
 *      / assignment
 *      / while_clause
 *      / do_clause
 *      / for_clause
 *      / if_clause
 *      / try_clause
 *      / switch_clause
 *      / break_statement
 *      / continue_statement
 *      / trace_statement
 *      / inline_statement
 *      / yield_statement
 *      / type_statement
 *      / exit_statement
 *      / print_statement
 *      / return_statement
 *      / raise_statement
 *      / function_body
 *      
 * @return ast_function_body_element* 
 * 
 */
ast_function_body_element* parse_function_body_element() {

    ENTER;
    ast_function_body_element* node = NULL;
    ast_node* nterm;
    void* post = post_token_queue();

    if((NULL != (nterm = (ast_node*)parse_var_definition())) ||
            (NULL != (nterm = (ast_node*)parse_function_reference())) ||
            (NULL != (nterm = (ast_node*)parse_create_reference())) ||
            (NULL != (nterm = (ast_node*)parse_destroy_reference())) ||
            (NULL != (nterm = (ast_node*)parse_assignment())) ||
            (NULL != (nterm = (ast_node*)parse_while_clause())) ||
            (NULL != (nterm = (ast_node*)parse_do_clause())) ||
            (NULL != (nterm = (ast_node*)parse_for_clause())) ||
            (NULL != (nterm = (ast_node*)parse_if_clause())) ||
            (NULL != (nterm = (ast_node*)parse_try_clause())) ||
            (NULL != (nterm = (ast_node*)parse_switch_clause())) ||
            (NULL != (nterm = (ast_node*)parse_break_statement())) ||
            (NULL != (nterm = (ast_node*)parse_continue_statement())) ||
            (NULL != (nterm = (ast_node*)parse_trace_statement())) ||
            (NULL != (nterm = (ast_node*)parse_inline_statement())) ||
            (NULL != (nterm = (ast_node*)parse_yield_statement())) ||
            (NULL != (nterm = (ast_node*)parse_type_statement())) ||
            (NULL != (nterm = (ast_node*)parse_exit_statement())) ||
            (NULL != (nterm = (ast_node*)parse_print_statement())) ||
            (NULL != (nterm = (ast_node*)parse_return_statement())) ||
            (NULL != (nterm = (ast_node*)parse_raise_statement())) ||
            (NULL != (nterm = (ast_node*)parse_function_body()))) {

        node = CREATE_AST_NODE(AST_function_body_element, ast_function_body_element);
        node->nterm = nterm;
    }
    else 
        reset_token_queue(post);

    RETV(node);
}

/**
 * @brief 
 * 
 *  break_statement
 *      = 'break'
 *      
 * @return ast_break_statement* 
 * 
 */
ast_break_statement* parse_break_statement() {

    ENTER;
    ast_break_statement* node = NULL;

    if(TOK_BREAK != TTYPE) {
        advance_token();
        node = CREATE_AST_NODE(AST_break_statement, ast_break_statement);
    }

    RETV(node);
}

/**
 * @brief 
 * 
 *  continue_statement
 *      = 'continue'
 *      
 * @return ast_continue_statement* 
 * 
 */
ast_continue_statement* parse_continue_statement() {

    ENTER;
    ast_continue_statement* node = NULL;

    if(TOK_CONTINUE == TTYPE) {
        advance_token();
        node = CREATE_AST_NODE(AST_continue_statement, ast_continue_statement);
    }

    RETV(node);
}

/**
 * @brief 
 * 
 *  trace_statement
 *      = 'trace'
 *      
 * @return ast_trace_statement* 
 * 
 */
ast_trace_statement* parse_trace_statement() {

    ENTER;
    ast_trace_statement* node = NULL;

    if(TOK_TRACE == TTYPE) {
        advance_token();
        node = CREATE_AST_NODE(AST_trace_statement, ast_trace_statement);
    }

    RETV(node);
}

/**
 * @brief 
 * 
 *  inline_statement
 *      = 'inline' '{' RAW_TEXT '}'
 *      
 * @return ast_inline_statement* 
 * 
 */
ast_inline_statement* parse_inline_statement() {

    ENTER;
    ast_inline_statement* node = NULL;

    if(TOK_INLINE == TTYPE) {
        advance_token();
        node = CREATE_AST_NODE(AST_inline_statement, ast_inline_statement);
        node->tok = get_token();
    }

    RETV(node);
}

/**
 * @brief 
 * 
 *  yield_statement
 *      = 'yield' '(' compound_reference ')'
 *      
 * @return ast_yield_statement* 
 * 
 */
ast_yield_statement* parse_yield_statement() {

    ENTER;
    ast_yield_statement* node = NULL;
    ast_compound_reference* ref;

    if(TOK_YIELD == TTYPE) {
        advance_token();
        if(TOK_OPAREN == TTYPE) {
            advance_token();
            if(NULL != (ref = parse_compound_reference())) {
                node = CREATE_AST_NODE(AST_yield_statement, ast_yield_statement);
                node->ref = ref;
                if(TOK_OPAREN == TTYPE) {
                    advance_token();
                }
                else
                    EXPECTED("a ')'");
            }
            else
                EXPECTED("a compound reference");
        }
        else
            EXPECTED("a '('");
    }

    RETV(node);
}

/**
 * @brief 
 * 
 *  type_statement
 *      = 'type' '(' compound_reference ')'
 *      
 * @return ast_type_statement* 
 * 
 */
ast_type_statement* parse_type_statement() {

    ENTER;
    ast_type_statement* node = NULL;
    ast_compound_reference* ref;

    if(TOK_TYPE == TTYPE) {
        advance_token();
        if(TOK_OPAREN == TTYPE) {
            advance_token();
            if(NULL != (ref = parse_compound_reference())) {
                node = CREATE_AST_NODE(AST_type_statement, ast_type_statement);
                node->ref = ref;
                if(TOK_OPAREN == TTYPE) {
                    advance_token();
                }
                else
                    EXPECTED("a ')'");
            }
            else
                EXPECTED("a compound reference");
        }
        else
            EXPECTED("a '('");
    }

    RETV(node);
}

/**
 * @brief 
 * 
 *  exit_statement
 *      = 'exit' '(' ( expression )? ')
 *      
 * @return ast_exit_statement* 
 * 
 */
ast_exit_statement* parse_exit_statement() {

    ENTER;
    ast_exit_statement* node = NULL;
    ast_expression* expr;

    if(TOK_EXIT == TTYPE) {
        advance_token();
        if(TOK_OPAREN == TTYPE) {
            advance_token();
            expr = parse_expression(); // optional
            if(TOK_OPAREN == TTYPE) {
                advance_token();
            }
            else {
                EXPECTED("a ')'");
                RETV(NULL);
            }
        }
        else {
            EXPECTED("a '('");
            RETV(NULL);
        }

        node = CREATE_AST_NODE(AST_exit_statement, ast_exit_statement);
        node->expr = expr;
    }

    RETV(node);
}

/**
 * @brief 
 * 
 *  print_statement
 *      = 'print' ( expression_list )?
 *      
 * @return ast_print_statement* 
 * 
 */
ast_print_statement* parse_print_statement() {

    ENTER;
    ast_print_statement* node = NULL;

    if(TOK_PRINT == TTYPE) {
        advance_token();
        node = CREATE_AST_NODE(AST_print_statement, ast_print_statement);
        node->elst = parse_expression_list(); // optional
    }

    RETV(node);
}

/**
 * @brief 
 * 
 *  return_statement
 *      = 'return' ( '(' ( expression )? ')' )?
 *      
 * @return ast_return_statement* 
 * 
 */
ast_return_statement* parse_return_statement() {

    ENTER;
    ast_return_statement* node = NULL;

    if(TOK_RETURN == TTYPE) {
        advance_token();
        node = CREATE_AST_NODE(AST_return_statement, ast_return_statement);
        if(TOK_OPAREN == TTYPE) { // optional
            advance_token();
            node->expr = parse_expression(); // optional
            if(TOK_CPAREN == TTYPE) { // required for oparen
                advance_token();
            }
            else {
                node = NULL;
                EXPECTED("a ')'");
            }
        }
    }

    RETV(node);
}

/**
 * @brief 
 * 
 *  raise_statement
 *      = 'raise' '(' SYMBOL ',' formatted_strg ')'
 *      
 * @return ast_raise_statement* 
 * 
 */
ast_raise_statement* parse_raise_statement() {

    ENTER;
    ast_raise_statement* node = NULL;
    Token* symb;
    ast_formatted_strg* str;

    if(TOK_RAISE == TTYPE) {
        advance_token();
        if(TOK_OPAREN == TTYPE) {
            advance_token();
            if(TOK_SYMBOL == TTYPE) {
                symb = get_token();
                advance_token();
                if(TOK_COMMA == TTYPE) {
                    advance_token();
                    if(NULL == (str = parse_formatted_strg())) {
                        EXPECTED("formatted string");
                        RETV(NULL);
                    }
                    else if(TOK_CPAREN == TTYPE) {
                        node = CREATE_AST_NODE(AST_raise_statement, ast_raise_statement);
                        node->str = str;
                        node->symb = symb;
                    }
                    else
                        EXPECTED("a ')'");
                }
                else
                    EXPECTED("a ','");
            }
            else
                EXPECTED("a SYMBOL");
        }
        else
            EXPECTED("a '('");
    }

    RETV(node);
}


