
#include <stdarg.h>

#include "common.h"

static size_t errors = 0;
static size_t warnings = 0;

void bump_error_count() { errors++; }

void syntax(const char* fmt, ...) {

    va_list args;

    fprintf(stderr, "error: %s: %d: %d: ", get_fname(), get_line_no(), get_col_no());
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    errors++;

    recover_from_error();
}

void warning(const char* fmt, ...) {

    va_list args;

    fprintf(stderr, "warning: %s: %d: %d: ", get_fname(), get_line_no(), get_col_no());
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    warnings++;
}

void info(const char* fmt, ...) {

    va_list args;

    fprintf(stderr, "info: ");
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    warnings++;
}

void fatal(const char* fmt, ...) {

    va_list args;

    fprintf(stderr, "fatal: ");
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    errors++;
    fprintf(stderr, "\n");
    exit(errors);
}

size_t num_errors() {
    return errors;
}

size_t num_warnings() {
    return warnings;
}
