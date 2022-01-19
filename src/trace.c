
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static int level = 0;  // no messages by default

void message(int lev,
            const char* file,
            const char* func,
            int line,
            const char* fmt, ...) {

    va_list args;

    if(lev <= level) {
        fprintf(stderr, "trace: %s: %s(): %d: ", file, func, line);
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
        fprintf(stderr, "\n");
    }
}

void set_msg_level(int lev) {

    level = lev;
}
