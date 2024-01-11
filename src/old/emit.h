#ifndef EMIT_H
#define EMIT_H

void set_emitter(FILE* fp);
void open_emitter(const char* fname);
void close_emitter();
void emit_preamble();
void emit_postamble();
void emit_line_number();

#endif
