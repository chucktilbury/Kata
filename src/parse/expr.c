/**
 * @file expr.c
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

// This is used to detect unary operators. If it is true, and an operator is 
// found then it could be unary. Only the '!' and the '-' can be unary, all 
// others are errors if the flag is set.
static bool flag = true;

static int get_prec(TokenType type) {

    switch(type) {
        case TOK_OR:
            return 10;
        case TOK_AND:
            return 20;
        case TOK_EQU:
        case TOK_NEQU:
            return 30;
        case TOK_LORE:
        case TOK_GORE:
        case TOK_OPBRACE:
        case TOK_CPBRACE:
            return 40;
        case TOK_ADD:
        case TOK_SUB:
            return 50;
        case TOK_DIV:
        case TOK_MUL:
        case TOK_MOD:
            return 60;
        case TOK_CARAT:
            return 70;
        case TOK_NOT:
        case TOK_UNARY_MINUS:
            return 80;
        default:
            // no precidence
            return 0; 
    }
}

static bool get_assoc(TokenType type) {

    switch(type) {
        case TOK_NOT:
        case TOK_UNARY_MINUS:
        case TOK_CARAT:
            return false;   // right to left
        default:
            return true;    // left to right
    }
}

/**
 * @brief 
 * 
 *  expression
 *      = expr_and 'or' expr_or
 *      / expr_equ 'and' expr_and
 *      / expr_mag '==' expr_equ
 *      / expr_mag '!=' expr_equ
 *      / expr_term '<' expr_mag
 *      / expr_term '>' expr_mag
 *      / expr_term '<=' expr_mag
 *      / expr_term '>=' expr_mag
 *      / expr_fact '+' expr_term
 *      / expr_fact '-' expr_term
 *      / expr_pow '*' expr_fact
 *      / expr_pow '/' expr_fact
 *      / expr_pow '%' expr_fact
 *      / expr_unary '^' expr_pow
 *      / '!' expr_primary
 *      / '-' expr_primary
 *      / '(' expression ')'
 *      
 * @return ast_expression* 
 * 
 */
ast_expression* parse_expression() {

    ENTER;
    ast_expression* node = NULL;
    PtrList* stack = create_ptr_list();
    PtrList* queue = create_ptr_list();
    ast_node* nterm;

    while(true) {
        if((NULL != (nterm = parse_operator())) ||
                (NULL != (nterm = parse_expr_primary()))) {

            if(AST_expr_primary == ast_node_type(nterm)) {

            }
            else {
                
            }
        }
        else 
            break;

    }

    RETV(node);
}

/**
 * @brief 
 * 
 *  ast_operator
 *      = AND
 *      / OR
 *      / '=='
 *      / '!='
 *      / '<'
 *      / '>'
 *      / '<='
 *      / '>='
 *      / '+'
 *      / '-'
 *      / '*'
 *      / '/'
 *      / '%'
 *      / '^'
 *      / UNARY_MINUS
 *      / '!'
 *      / '('
 *      / ')'
 *      
 * @return ast_operator* 
 * 
 */
ast_operator* parse_operator() {

    ENTER;
    ast_operator* node = NULL;
    Token* tok = get_token();

    switch(token_type(tok)) {
        case TOK_AND:
        case TOK_OR:
        case TOK_LORE:
        case TOK_GORE:
        case TOK_OPBRACE:
        case TOK_CPBRACE:
        case TOK_EQU:
        case TOK_NEQU:
        case TOK_ADD:
        case TOK_ASSIGN:
        case TOK_DIV:
        case TOK_MUL:
        case TOK_MOD:
        case TOK_OPAREN:
        case TOK_CARAT:
            // these operators must not be unary
            if(!flag) {
                node = CREATE_AST_NODE(AST_operator, ast_operator);
                node->tok = tok;
                finalize_token();
                advance_token();
                flag = true;
            }
            else
                show_syntax_error("the %s operator must not be unary", tok_to_str(tok));
            break;

        case TOK_SUB:
            // the '-' can be unary
            node = CREATE_AST_NODE(AST_operator, ast_operator);
            node->tok = tok;
            if(flag)
                node->tok = TOK_UNARY_MINUS;
            finalize_token();
            advance_token();
            flag = true;
            break;

        case TOK_NOT:
            // the '!' or 'not' must be unary
            if(flag) {
                node = CREATE_AST_NODE(AST_operator, ast_operator);
                node->tok = tok;
                finalize_token();
                advance_token();
                flag = true;
            }
            else 
                show_syntax_error("the not operator must be unary");
            break;

        case TOK_CPAREN:
            node = CREATE_AST_NODE(AST_operator, ast_operator);
            node->tok = tok;
            finalize_token();
            advance_token();
            flag = false;
            break;
    }


    RETV(node);
}

/**
 * @brief 
 * 
 *  cast_statement
 *      = type_name ':' expression 
 * 
 * @return ast_cast_statement* 
 * 
 */
ast_cast_statement* parse_cast_statement() {

    ENTER;
    ast_cast_statement* node = NULL;
    void* post = post_token_queue();
    ast_type_name* type;
    ast_expression* expr;

    if(NULL != (type = parse_type_name())) {
        if(TOK_COLON == token_type(get_token)) {
            finalize_token();
            advance_token();

            if(NULL != (expr = parse_expression())) {
                node = CREATE_AST_NODE(AST_cast_statement, ast_cast_statement);
                node->type = type;
                node->expr = expr;
            }
            else
                EXPECTED("an expression");
        }
        else // not a cast statement, not an error
            reset_token_queue(post);
    }
    else // not a cast, not an error
        reset_token_queue(post);

    RETV(node);
}

/**
 * @brief 
 * 
 *  expr_primary
 *      = literal_value
 *      / compound_reference
 *      / cast_statement
 *      
 * @return ast_expr_primary* 
 * 
 */
ast_expr_primary* parse_expr_primary() {

    ENTER;
    ast_expr_primary* node = NULL;
    ast_node* nterm;
    void* post = post_token_queue();

    if((NULL != (nterm = parse_literal_value())) ||
            (NULL != (nterm = parse_compound_reference())) ||
            (NULL != (nterm = parse_cast_statement()))) {
        
        node = CREATE_AST_NODE(AST_expr_primary, ast_expr_primary);
        node->nterm = nterm;
        flag = false;
        finalize_token_queue();
    }
    else
        reset_token_queue(post);

    RETV(node);
}

/**
 * @brief 
 * 
 *  expression_list
 *      = '(' (expression ( ',' expression )*)? ')'
 *      
 * @return ast_expression_list* 
 * 
 */
ast_expression_list* parse_expression_list() {

    ENTER;
    ast_expression_list* node = NULL;

    RETV(node);
}

/**
 * @brief 
 * 
 *  assignment_item
 *      = expression
 *      / list_init
 *      / dict_init
 *      / string_expr
 *      / cast_statement
 *      
 * @return ast_assignment_item* 
 * 
 */
ast_assignment_item* parse_assignment_item() {

    ENTER;
    ast_assignment_item* node = NULL;
    ast_node* nterm;
    void* post = post_token_queue();

    if((NULL != (nterm = parse_expression())) ||
            (NULL != (nterm = parse_list_init())) ||
            (NULL != (nterm = parse_dict_init())) ||
            (NULL != (nterm = parse_string_expr())) ||
            (NULL != (nterm = parse_cast_statement()))) {

        node = CREATE_AST_NODE(AST_assignment_item, ast_assignment_item);
        node->nterm = nterm;
        finalize_token_queue();
    }
    else
        reset_token_queue(post);

    RETV(node);
}

/**
 * @brief 
 * 
 *  assignment
 *      = compound_reference '=' assignment_item
 *      / compound_reference '+=' expression
 *      / compound_reference '-=' expression
 *      / compound_reference '*=' expression
 *      / compound_reference '/=' expression
 *      / compound_reference '%=' expression
 *      
 * @return ast_assignment* 
 * 
 */
ast_assignment* parse_assignment() {

    ENTER;
    ast_assignment* node = NULL;

    RETV(node);
}


