/**
 * @file scanner.h
 *
 * @brief Scanner header file. The items in this header must be implemented by
 * the scanner and the scanner queue. These routines are used directly by the
 * parser.
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 01-07-2024
 * @copyright Copyright (c) 2024
 */
#ifndef _SCANNER_H
#define _SCANNER_H

/*
 * This header contains a sorted list of the actual keywords that are scanned
 * in the scanner.
 */
#include "keyword.h"

/**
 * @brief The parser expects a token to look like this.
 */
typedef struct {
    Str* str;       // String that caused the token to be recognized
    TokenType type; // Type of the token
    int line_no;    // Line number where the token was recognized
    int col_no;     // Column of the last character of the token
    const char* fname;  // File name where the token was taken
    bool used;      // When the token has been finalized in a rule.
} Token;

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
 * global in the scanner. If the value of this token needs to be preserved,
 * then the token should be copied.
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
 * @brief Make the next token in the stream the current token. If the token
 * before this one was the end of the input, then nothing happens and the
 * returned token is the end of input token. Returns the current token after
 * the advance happens.
 *
 * @return Token*
 */
Token* advance_token();

/**
 * @brief Mark this token as having been used to compose a rule.
 *
 * @param tok
 *
 */
void finalize_token(Token* tok);

/**
 * @brief Reset the head of the token queue to the first token that has not
 * been marked as being finalized.
 */
void finalize_token_queue();

/**
 * @brief Reset the token queue to the state that it was in when the token was
 * read. It takes the token that is supplied as a parameter and makes it the
 * crnt token so that it is read as the next one.
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
 * @brief Iterate the token queue. This is used by consumers that require raw
 * access to the token queue. That includes things like error reporting and
 * recovery. The first time that the iterator is called, the parameter needs
 * to be NULL. The current pointer in the queue is store there in order to
 * track the location in the queue. When there are no more elements in the
 * queue, then the return value is NULL.
 *
 * @param mark
 * @return Token*
 */
Token* iterate_token_queue(void** mark);

/**
 * @brief Initialize the scanner data structures. This must be called before
 * any other calls into the scanner.
 */
void init_scanner();

/**
 * @brief Convert the binary token type to a string for display.
 *
 * @param type
 * @return const char*
 */
const char* tok_to_str(TokenType type);

#endif

