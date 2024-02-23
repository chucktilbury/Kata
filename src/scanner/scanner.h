/**
 * @file scanner.h
 *
 * @brief Scanner header file. The items in this header must be implemented
 * by the scanner and the scanner queue. These routines are used directly
 * by the parser.
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 01-07-2024
 * @copyright Copyright (c) 2024
 */
#ifndef _SCANNER_H
#define _SCANNER_H

#include "util.h"

/*
 * This header contains a sorted list of the actual keywords that are
 * scanned in the scanner.
 */
//#include "keyword.h"
#include "tokens.h"

/**
 * @brief Open a file for the scanner to read from. Files are expected to be
 * opened in a stack so that when a file is opened the input stream is
 * switched. Files are automatically closed when the last character is read.
 *
 * @param fname
 */
void open_file(const char* fname);

/**
 * @brief Close the current file and pop it off of the file stack.
 */
void close_file();

/**
 * @brief Get the token object. This returns the current token, which is a
 * global in the scanner. If the pointer to this token needs to be referenced,
 * after advance_token() is called, then the token should be copied.
 *
 * @return Token*
 */
Token* get_token();

/**
 * @brief Do a deep copy of the given token pointer. All memory is duplicated.
 *
 * @param tok
 * @return Token*
 */
Token* copy_token(const Token* tok);

/**
 * @brief Make the next token in the stream the current token. This could
 * return a token that has already been read if the queue was reset, or a
 * new token could be read, depending on the state of the queue as set by
 * the other functions.
 *
 * @return Token*
 */
Token* advance_token();

/**
 * @brief Mark this token as having been used to compose a rule.
 */
void finalize_token();

/**
 * @brief Reset the head of the token queue to the first token that has not
 * been marked as being finalized. This actually discards the tokens that
 * have been consumed. This is called when a valid non-terminal is
 * recognized by the parser.
 */
void finalize_token_queue();

/**
 * @brief Reset the token queue to the state that it was in before this parser
 * function was called. This is used when the tokens that were consumed by
 * this function do not produce a valid parse. That may or may not be an
 * error. In the case of an error, this should not be called because the error
 * recovery function needs to know which tokens are involved in the error and
 * that data is captured by the flags this function undoes.
 *
 * @param tok
 */
void reset_token_queue(void* crnt);

/**
 * @brief Grab the current queue pointer so that it can be reset when the
 * crnt pointer moves as a result of parsing the line. This is used in
 * conjunction with reset_token_queue().
 *
 * @return void*
 */
void* post_token_queue();

/**
 * @brief Iterate the token queue. This is used by consumers that require
 * raw access to the token queue. That includes things like error reporting
 * and recovery. The first time that the iterator is called, the parameter
 * needs to be NULL. The current pointer in the queue is store there in
 * order to track the location in the queue. When there are no more
 * elements in the queue, then the return value is NULL.
 *
 * @param mark
 * @return Token*
 */
Token* iterate_token_queue(void** mark);

/**
 * @brief Initialize the scanner data structures. This must be called
 * before any other calls into the scanner.
 */
void init_scanner();

/**
 * @brief Convert the binary token type to a string for display.
 *
 * @param type
 * @return const char*
 */
//const char* tok_to_str(TokenType type);

/**
 * @brief This function literally discards the entire token queue. This
 * is used in error recovery to discard tokens that are a part of the
 * error so that parsing can resume.
 *
 */
void discard_token_queue();

void print_token(Token* tok);
#endif
