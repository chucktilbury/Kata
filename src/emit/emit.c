#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "scanner.h"
#include "errors.h"

static FILE* outf = NULL;

void set_emitter(FILE* fp) {

    outf = fp;
}

void open_emitter(const char* fname) {

    if(NULL == (outf = fopen(fname, "w")))
        fatal("cannot open output file: %s: %s\n", fname, strerror(errno));
}

void close_emitter() {

    fclose(outf);
}

void emit_preamble() {
    MSG("emit preamble");
    fprintf(outf, "// Start of preamble\n");
    fprintf(outf, "// End of preamble\n");
}

void emit_postamble() {
    MSG("emit post amble");
    fprintf(outf, "// Start of postamble\n");
    fprintf(outf, "// End of postamble\n");
}

/*
 * https://gcc.gnu.org/onlinedocs/gcc-10.2.0/cpp/Preprocessor-Output.html
 */
void emit_line_number() {

    fprintf(outf, "# %d \"%s\"\n", get_line_no(), get_fname());
}
