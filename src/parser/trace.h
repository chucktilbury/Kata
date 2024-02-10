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

#include "scanner.h"
extern void print_token(Token*);
extern int trace_count;
extern const int trace_increment;

#define SET_TRACE_COUNT(v)                 \
    do {                                   \
        trace_count = (v)*trace_increment; \
    } while(false)

#define ENTER                                                                \
    do {                                                                     \
        printf("%*senter: %s(): %d\n", trace_count, "", __func__, __LINE__); \
        trace_count += trace_increment;                                      \
    } while(false)

#define TRACE(f, ...)                                  \
    do {                                               \
        printf("%*s*%d: ", trace_count, "", __LINE__); \
        printf(f __VA_OPT__(, ) __VA_ARGS__);          \
        printf("\n");                                  \
    } while(false)

#define TRACE_TERM(t)                                  \
    do {                                               \
        printf("%*s*%d: ", trace_count, "", __LINE__); \
        print_token(t);                                \
        printf("\n");                                  \
    } while(false)

#define TRACE_NTERM(n)                                 \
    do {                                               \
        printf("%*s*%d: ", trace_count, "", __LINE__); \
        print_nonterminal(n);                          \
        printf("\n");                                  \
    } while(false)

#define RETV(v)                                                                                     \
    do {                                                                                            \
        trace_count -= trace_increment;                                                             \
        if((void*)(v) == NULL)                                                                      \
            printf("%*sreturn(NULL): %s(): %d\n", trace_count, "", __func__, __LINE__);             \
        else                                                                                        \
            printf("%*sreturn(%p): %s(): %d\n", trace_count, "", ((void*)(v)), __func__, __LINE__); \
        return (v);                                                                                 \
    } while(false)

#define RET                                                                     \
    do {                                                                        \
        trace_count -= trace_increment;                                         \
        printf("%*sreturn(): %s(): %d\n", trace_count, "", __func__, __LINE__); \
        return;                                                                 \
    } while(false)

#define EXIT(v)                                                                           \
    do {                                                                                  \
        printf("%*sexit(%d): %s: %d\n", trace_count, "", ((int)(v)), __func__, __LINE__); \
    } while(false)

#else

#define SET_TRACE_COUNT(v)
#define ENTER
#define TRACE(f, ...)
#define TRACE_TERM(t)
#define TRACE_NTERM(n)
#define RETV(v) return v
#define RET return
#define EXIT(v) exit(v)

#endif

#endif /* _TRACE_H */
