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
#ifndef _ERRORS_H
#define _ERRORS_H

void handle_error(const char* fmt, ...);
void handle_warning(const char* fmt, ...);
int get_num_errors();
int get_num_warnings();

#if USE_TRACE
static int count = 0;
static const int increment = 4;
#define ENTER do { \
        printf("%*senter: %s(): %d\n", count, "", __func__, __LINE__); \
        count+=increment; \
    } while(false)

#define TRACE(f, ...) do { \
        printf("%*s*%d: ", count, "", __LINE__); \
        printf(f __VA_OPT__(,) __VA_ARGS__); \
        printf("\n"); \
    } while(false)

#define RETV(v) do { \
        count-=increment; \
        if((void*)(v) == NULL) \
            printf("%*sreturn(NULL): %s(): %d\n", count, "", __func__, __LINE__); \
        else \
            printf("%*sreturn(%p): %s(): %d\n", count, "", ((void*)(v)), __func__, __LINE__); \
        return (v); \
    } while(false)

#define RET do { \
        count-=increment; \
        printf("%*sreturn(): %s(): %d\n", count, "", __func__, __LINE__); \
        return; \
    } while(false)

#define EXIT(v) do { \
        printf("%*sexit(%d): %s: %d\n", count, "", ((int)(v)), __func__, __LINE__); \
    } while(false)

#else
#define ENTER
#define TRACE(f, ...)
#define RETV(v) return v
#define RET return
#define EXIT(v) exit(v)
#endif

#endif /* _ERRORS_H */
