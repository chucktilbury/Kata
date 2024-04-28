/**
 * @file errors.c
 *
 * @brief Handle parser error reporting and recovery.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 01-13-2024
 * @copyright Copyright (c) 2024
 */
#include <stdlib.h>

#include "errors.h"
#include "fileio.h"
#include "scanner.h"
#include "trace.h"

#include <stdarg.h>

static int num_errors   = 0;
static int num_warnings = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough="
/*
 * Recover from a syntax error. This discards tokens until it's reasonably 
 * possible to resume parsing. 
 * 
 */
static void recover_error() {

    ENTER;

    //dump_token_queue();

    bool finished = false;
    while(!finished) {
        TRACE_TERM(get_token());
        switch(TTYPE) {
            case TOK_CPAREN:
            case TOK_CCBRACE:
            case TOK_CSBRACE:
                // consume the token and continue
                advance_token();
                finished = true;
                break;
            case TOK_OPAREN:
            case TOK_OCBRACE:
            case TOK_OSBRACE:
                // continue parsing from where the error occoured
                finished = true;
                break;
            case TOK_FUNC:
            case TOK_FUNCTION:
            case TOK_VAR:
            case TOK_VARIABLE:
            case TOK_CLASS:
            case TOK_NAMESPACE:
            case TOK_IMPORT:
            case TOK_END_OF_FILE:
            case TOK_END_OF_INPUT:
                // break
                finished = true;
                break;
            default:
                // simply consume the current token.
                advance_token();
                break;
        }
    }

    kill_token_queue();
    set_recovery_state(true);
    
    //dump_token_queue();

    RET;
}
#pragma GCC diagnostic pop

/**
 * @brief Abort the program.
 *
 * @param str
 * @param ...
 *
 */
void fatal_error(const char* str, ...) {

    va_list args;

    fprintf(stderr, "Internal Error: ");

    va_start(args, str);
    vfprintf(stderr, str, args);
    va_end(args);
    fputc('\n', stderr);
    num_errors++;
    exit(1);
}

/**
 * @brief Top level function to handle a generic error inside the parser.
 * It also is the interface to error recovery.
 *
 * @param fmt
 * @param ...
 *
 */
void show_syntax(const char* fmt, ...) {

    va_list args;

    fprintf(stderr, "Syntax: %s: %d: %d: ", get_fname(), get_line_no(), get_col_no());

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fputc('\n', stderr);
    num_errors++;
    recover_error();
}

/**
 * @brief Report a generic warning.
 *
 * @param fmt
 * @param ...
 *
 */
void show_warning(const char* fmt, ...) {

    va_list args;

    fprintf(stderr, "Warning: %s: %d: %d: ", get_fname(), get_line_no(), get_col_no());

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fputc('\n', stderr);
    num_warnings++;
}

/**
 * @brief Get the num errors object
 *
 * @return int
 *
 */
int get_num_errors() {

    return num_errors;
}

/**
 * @brief Get the num warnings object
 *
 * @return int
 *
 */
int get_num_warnings() {

    return num_warnings;
}