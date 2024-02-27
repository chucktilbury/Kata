/**
 * @file trace.h
 *
 * @brief This is used for debugging.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 02-03-2024
 * @copyright Copyright (c) 2024
 */
#ifndef _TRACE_H
#define _TRACE_H

#ifdef USE_TRACE

#include "ast.h"
#include "scanner.h"
void print_nonterminal(ast_node* node);
void print_token(Token* tok);

#define TRACE_TERM(t)                     \
    do {                                  \
        PAD;                              \
        printf("TERM: %s(): ", __func__); \
        print_token(t);                   \
        CAP;                              \
    } while(false)

#define TRACE_NTERM(n)                        \
    do {                                      \
        PAD;                                  \
        printf("NON-TERM: %s(): ", __func__); \
        print_nonterminal(n);                 \
        CAP;                                  \
    } while(false)

#else

#define TRACE_TERM(t)
#define TRACE_NTERM(n)

#endif

#endif /* _TRACE_H */
