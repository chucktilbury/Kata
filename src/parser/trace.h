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

#define TRACE_TERM(t)                                                    \
    TRACE("terminal: %s: \"%s\": %d: %d: \"%s\"", tok_to_str((t)->type), \
          raw_string((t)->str), (t)->line_no, (t)->col_no, (t)->fname)

#include "ast.h"
void print_nonterminal(AstNode* node);
extern int trace_count;

#define TRACE_NTERM(n)                                 \
    do {                                               \
        printf("%*s*%d: ", trace_count, "", __LINE__); \
        print_nonterminal(n);                          \
        printf("\n");                                  \
    } while(false)

#else

#define TRACE_TERM(t)
#define TRACE_NTERM(n)

#endif

#endif /* _TRACE_H */
