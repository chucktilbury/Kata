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
#ifndef __TRACE_H__
#define __TRACE_H__

#ifdef USE_TRACE
#include <stdbool.h>
#include <stdio.h>

extern int trace_count;
extern const int trace_increment;
void push_trace_state(bool state);
void set_trace_state(bool state);
bool pop_trace_state();
bool peek_trace_state();

void print_nonterminal(void* node);
void print_terminal(void* ptr);

#define PAD printf("%*s", trace_count, "")

#define CAP fputc('\n', stdout);

#define TRACE(f, ...)                              \
    do {                                           \
        if(peek_trace_state()) {                   \
            PAD;                                   \
            printf("TRACE: %s(): ", __func__);     \
            printf((f)__VA_OPT__(, ) __VA_ARGS__); \
            CAP;                                   \
        }                                          \
    } while(false)

#define ENTER                                  \
    do {                                       \
        if(peek_trace_state()) {               \
            PAD;                               \
            printf("ENTER: %s(): ", __func__); \
            trace_count += trace_increment;    \
            CAP;                               \
        }                                      \
    } while(false)

#define RET                                     \
    do {                                        \
        if(peek_trace_state()) {                \
            trace_count -= trace_increment;     \
            PAD;                                \
            printf("RETURN: %s(): ", __func__); \
            CAP;                                \
        }                                       \
        return;                                 \
    } while(false)

#define RETV(v)                                       \
    do {                                              \
        if(peek_trace_state()) {                      \
            trace_count -= trace_increment;           \
            PAD;                                      \
            printf("RETURN(%s): %s()", #v, __func__); \
            CAP;                                      \
        }                                             \
        return (v);                                   \
    } while(false)

#define EXIT(n)                                   \
    do {                                          \
        if(peek_trace_state()) {                  \
            PAD;                                  \
            printf("EXIT(#n): %s(): ", __func__); \
            CAP;                                  \
        }                                         \
        exit(v);                                  \
    } while(false)

#define PUSH_TRACE_STATE(s)                                    \
    do {                                                       \
        push_trace_state(s);                                   \
        TRACE("push trace state: %s", (s) ? "true" : "false"); \
    } while(false)

#define SET_TRACE_STATE(s)                                    \
    do {                                                      \
        set_trace_state(s);                                   \
        TRACE("set trace state: %s", (s) ? "true" : "false"); \
    } while(false)

#define POP_TRACE_STATE()         \
    do {                          \
        pop_trace_state();        \
        TRACE("pop trace state"); \
    } while(false)

#define TRACE_TERM(t)                         \
    do {                                      \
        if(peek_trace_state()) {              \
            PAD;                              \
            printf("TERM: %s(): ", __func__); \
            print_terminal(t);                \
            CAP;                              \
        }                                     \
    } while(false)

#define TRACE_NTERM(n)                            \
    do {                                          \
        if(peek_trace_state()) {                  \
            PAD;                                  \
            printf("NON-TERM: %s(): ", __func__); \
            print_nonterminal(n);                 \
            CAP;                                  \
        }                                         \
    } while(false)

#else

#define TRACE(f, ...)
#define ENTER
#define EXIT(n) exit(n)
#define RET return
#define RETV(v) return (v)
#define PUSH_TRACE_STATE(s)
#define POP_TRACE_STATE()
#define SET_TRACE_STATE(s)

#define TRACE_TERM(t)
#define TRACE_NTERM(n)

#endif

#endif /* __TRACE_H__ */
