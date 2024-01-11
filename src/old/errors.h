#ifndef ERRORS_H
#define ERRORS_H

void syntax(const char* fmt, ...);
void warning(const char* fmt, ...);
void fatal(const char* fmt, ...);
void info(const char* fmt, ...);
size_t num_errors();
size_t num_warnings();
void bump_error_count();

#include "trace.h"

#endif
