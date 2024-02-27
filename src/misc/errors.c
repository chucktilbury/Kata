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
#include "errors.h"
#include "scanner.h"
#include "util.h"

#include <stdarg.h>

static int num_errors   = 0;
static int num_warnings = 0;

/*
 * Recover from a syntax error. This discards the current token queue and
 * reads five new tokens.
 */
static void recover_error() {

    discard_token_queue();
    for(int i = 0; i < 5; i++)
        advance_token();
    // reset_token_queue();
}

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
void show_syntax_error(const char* fmt, ...) {

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