/**
 * @file fileio.h
 *
 * @brief
 *
 * @author Charles Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 03-29-2024
 * @copyright Copyright (c) 2024
 */
#ifndef __FILEIO_H__
#define __FILEIO_H__

#define END_OF_FILE -1
#define END_OF_INPUT -2

void push_input_file(const char* fname);
void pop_input_file();
int get_char();
int consume_char();

int get_line_no();
int get_col_no();
const char* get_fname();

// Since multiple output files can be open in a moment, then it is accessed
// using an opaque handle.
// typedef void* FPTR;
// FPTR open_output_file(const char* fname);
// void close_output_file(FPTR fp);

// void emit_buf(FPTR h, void* buf, unsigned int size);
// void emit_fmt(FPTR h, const char* fmt, ...);
// void emit_Str(FPTR h, Str* str);
// void emit_str(FPTR h, const char* str);

#endif /* __FILEIO_H__ */
