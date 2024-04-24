/**
 * @file errors.h
 *
 * @brief Handle parser errors and error recovery.
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 01-13-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __ERRORS_H__
#define __ERRORS_H__

void show_syntax(const char* fmt, ...);
void show_warning(const char* fmt, ...);
void fatal_error(const char* fmt, ...);
int get_num_errors();
int get_num_warnings();

#define EXPECTED(s)                                                          \
    do {                                                                     \
        show_syntax("expected %s but got %s", (s), tok_to_str(get_token())); \
    } while(false)

#define SYNTAX(s, ...) \
    do { \
        show_syntax((s)__VA_OPT__(, ) __VA_ARGS__); \
    } while(false)

#define WARNING(s, ...) \
    do { \
        show_warning((s)__VA_OPT__(, ) __VA_ARGS__); \
    } while(false)

#define FATAL(s, ...) \
    do { \
        fatal_error((s)__VA_OPT__(, ) __VA_ARGS__); \
    } while(false)

#endif /* __ERRORS_H__ */
