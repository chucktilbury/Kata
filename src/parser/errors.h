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

void show_syntax_error(const char* fmt, ...);
void show_warning(const char* fmt, ...);
int get_num_errors();
int get_num_warnings();

#endif /* _ERRORS_H */
