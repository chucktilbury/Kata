/**
 * @file tok_queue.c
 *
 * @brief This is the token queue implementation. The token queue is the
 * public interface into the scanner and it's used by the parser to
 * implement rule matching.
 *
 * NOTE: This library expects to be using garbage collection.
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 01-07-2024
 * @copyright Copyright (c) 2024
 */
#include "scanner.h"
#include "util.h"

extern Token* scan_token();
static unsigned serial = 0;

typedef struct _tok_queue_item_ {
    Token* tok;      // the actual token pointer.
    unsigned serial; // serial number of the token.
    bool used;       // When the token has been finalized in a rule.
    struct _tok_queue_item_* next;
} TokQueueItem;

typedef struct {
    TokQueueItem* head;
    TokQueueItem* crnt;
    TokQueueItem* tail;
} TokQueue;

static TokQueue* tqueue = NULL;

/*
 * Append a token to the end of the queue. It could be that advance_token()
 * has found the end of the queue, but it could be something else.
 */
static void append_token(Token* tok) {

    assert(tqueue != NULL);
    assert(tok != NULL);

    TokQueueItem* item;

    TRY {
        item         = _ALLOC_T(TokQueueItem);
        item->tok    = copy_token(tok);
        item->serial = serial++;
        item->used   = false;
    }
    ANY_EXCEPT() {
        fprintf(stderr, "Fatal ");
        fprintf(stderr, "%s\n", EXCEPTION_MSG);
        exit(1);
    }
    FINAL

    if(tqueue->tail != NULL) {
        tqueue->tail->next = item;
        tqueue->tail       = item;
    }
    else {
        tqueue->head = item;
        tqueue->crnt = item;
        tqueue->tail = item;
    }
}

/**
 * @brief Open a file for the scanner to read from. Files are expected to be
 * opened in a stack so that when a file is opened the input stream is
 * switched. Files are automatically closed when the last character is read.
 *
 * @param fname
 */
void open_file(const char* fname) {

    // calling into the util library
    TRY {
        push_input_file(fname);
    }
    EXCEPT(FILE_ERROR) {
        fprintf(stderr, "Fatal ");
        fprintf(stderr, "%s\n", EXCEPTION_MSG);
        exit(1);
    }
    FINAL

    if(tqueue == NULL) {
        TRY {
            // prime the token pipeline
            tqueue = _ALLOC_T(TokQueue);
            append_token(scan_token());
        }
        EXCEPT(MEMORY_ERROR) {
            fprintf(stderr, "Fatal ");
            fprintf(stderr, "%s\n", EXCEPTION_MSG);
            exit(1);
        }
        FINAL
    }
}

/**
 * @brief Close the current file and pop it off of the file stack.
 */
void close_file() {

    pop_input_file();
}

/**
 * @brief Mark this token as having been used to compose a rule.
 *
 * @param tok
 *
 */
void finalize_token() {

    tqueue->crnt->used = true;
}

/**
 * @brief Reset the token queue to the state that it was in before this parser
 * function was called. This is used when the tokens that were consumed by
 * this function do not produce a valid parse. That may or may not be an
 * error. In the case of an error, this should not be called because the error
 * recovery function needs to know which tokens are involved in the error and
 * that data is captured by the flags this function undoes.
 */
void finalize_token_queue() {

    TokQueueItem* ptr = tqueue->head;
    while(ptr != NULL) {
        if(ptr->used)
            ptr = ptr->next;
        else
            break;
    }

    if(ptr != NULL)
        tqueue->head = ptr;
}

/**
 * @brief Get the token object. This returns the current token, which is a
 * global in the scanner. If the value of this token needs to be preserved,
 * then the token should be copied.
 *
 * @return Token*
 */
Token* get_token() {

    assert(tqueue != NULL);

    if(tqueue->crnt != NULL) {
        return tqueue->crnt->tok;
    }

    // Should never happen. Will probably cause a segfault.
    return NULL;
}

/**
 * @brief Do a deep copy of the given token pointer. All memory is duplicated.
 *
 * @param tok
 * @return Token*
 */
Token* copy_token(const Token* tok) {

    assert(tok != NULL);

    Token* ntok;

    TRY {
        ntok          = _ALLOC_T(Token);
        ntok->fname   = _DUP_STR(tok->fname);
        ntok->str     = copy_string(tok->str);
        ntok->line_no = tok->line_no;
        ntok->col_no  = tok->col_no;
        ntok->type    = tok->type;
    }
    ANY_EXCEPT() {
        // any exception is a fatal error:
        fprintf(stderr, "Fatal ");
        fprintf(stderr, "%s\n", EXCEPTION_MSG);
        exit(1);
    }
    FINAL

    return ntok;
}

/**
 * @brief Make the next token in the stream the current token. If the token
 * before this one was the end of the input, then nothing happens and the
 * returned token is the end of input token. Returns the current token
 * after the advance happens.
 *
 * @return Token*
 */
Token* advance_token() {

    // avoid stupid programmer tricks
    assert(tqueue != NULL);
    assert(tqueue->crnt != NULL);

    if(tqueue->crnt->tok->type != TOK_END_OF_INPUT) {
        if(tqueue->crnt->next == NULL)
            append_token(scan_token());

        tqueue->crnt = tqueue->crnt->next;
    }

    return tqueue->crnt->tok;
}

/**
 * @brief Grab the current queue pointer so that it can be reset when the
 * crnt pointer moves as a result of parsing the line. This is used in
 * conjunction with reset_token_queue(). This should be called and the pointer
 * saved at the beginning of any parser routine that expects alternatives.
 *
 * @return void*
 *
 */
void* post_token_queue() {

    assert(tqueue != NULL);

    return (void*)tqueue->crnt;
}

/**
 * @brief Reset the token stream to the beginning. This is used when a rule
 * could not be matched and the token stream needs to be rewound to test
 * the next rule in a list of alternatives. Call this when a parser
 * alternative fails in order to reset the crnt pointer.
 *
 * @return Token*
 */
void reset_token_queue(void* post) {

    assert(tqueue != NULL);

    TokQueueItem* tmp = (TokQueueItem*)post;
    unsigned end      = tqueue->crnt->serial;

    while(tmp->serial < end) {
        tmp->used = false;
        tmp       = tmp->next;
    }

    tqueue->crnt = (TokQueueItem*)post;
}

/**
 * @brief Iterate the token queue. This is used by consumers that require
 * raw access to the token queue. That includes things like error reporting
 * and recovery. The first time that the iterator is called, the parameter
 * needs to be NULL. The current pointer in the queue is store there in
 * order to track the location in the queue. When there are no more
 * elements in the queue, then the return value is NULL.
 *
 * Example:
 * void* mark = NULL;
 * for(Token* tok = iterate_tokens(&mark);
 *            tok != NULL;
 *            tok = iterate_tokens(&mark)) {
 *     // do stuff with tok
 * }
 *
 * @param mark
 * @return Token*
 */
Token* iterate_token_queue(void** mark) {

    assert(tqueue != NULL);
    assert(tqueue->head != NULL);

    if(*mark == NULL)
        *mark = tqueue->head;
    else if(((TokQueueItem*)(*mark))->next != NULL)
        *mark = ((TokQueueItem*)(*mark))->next;
    else
        return NULL;

    return ((TokQueueItem*)(*mark))->tok;
}

/**
 * @brief This function literally discards the entire token queue. This
 * is used in error recovery to discard tokens that are a part of the
 * error so that parsing can resume.
 *
 */
void discard_token_queue() {

    assert(tqueue != NULL);
    assert(tqueue->head != NULL);

    // Get a new token.
    advance_token();
    // Make the new token the head of the queue and the crnt item.
    tqueue->head = tqueue->crnt = tqueue->tail;
}
