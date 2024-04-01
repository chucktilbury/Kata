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
#include <assert.h>

#include "fileio.h"
#include "link_list.h"
#include "memory.h"
#include "scanner.h"
#include "trace.h"

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

// static TokQueue* tqueue = NULL;
static LinkList* tqueue_stack = NULL;

/**
 * @brief Push the token queue.
 *
 *
 */
void push_token_queue() {

    TokQueue* queue = _ALLOC_T(TokQueue);
    push_link_list(tqueue_stack, queue);
    append_token(scan_token());
}

/**
 * @brief Pop the token queue
 *
 *
 */
void pop_token_queue() {

    pop_link_list(tqueue_stack);
}

/*
 * Append a token to the end of the queue. It could be that advance_token()
 * has found the end of the queue, but it could be something else.
 */
void append_token(Token* tok) {

    TokQueue* tqueue = peek_link_list(tqueue_stack);
    assert(tqueue != NULL);
    assert(tok != NULL);

    TokQueueItem* item;

    item         = _ALLOC_T(TokQueueItem);
    item->tok    = copy_token(tok);
    item->serial = serial++;
    item->used   = false;

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

    ENTER;

    if(tqueue_stack == NULL)
        tqueue_stack = create_link_list();

    push_input_file(fname);
    push_token_queue();

    RET;
}

/**
 * @brief Close the current file and pop it off of the file stack.
 */
void close_file() {

    ENTER;
    pop_input_file();
    pop_link_list(tqueue_stack);
    RET;
}

/**
 * @brief Get the token object. This returns the current token, which is a
 * global in the scanner. If the value of this token needs to be preserved,
 * then the token should be copied.
 *
 * @return Token*
 */
Token* get_token() {

    TokQueue* tqueue = peek_link_list(tqueue_stack);
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

    ntok          = _ALLOC_T(Token);
    ntok->fname   = _DUP_STR(tok->fname);
    ntok->str     = copy_string(tok->str);
    ntok->line_no = tok->line_no;
    ntok->col_no  = tok->col_no;
    ntok->type    = tok->type;

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

    ENTER;
    // avoid stupid programmer tricks
    TokQueue* tqueue = peek_link_list(tqueue_stack);
    assert(tqueue != NULL);
    assert(tqueue->crnt != NULL);

    if(tqueue->crnt->tok->type != TOK_END_OF_INPUT) {
        if(tqueue->crnt->next == NULL)
            append_token(scan_token());

        tqueue->crnt = tqueue->crnt->next;
    }

    RETV(tqueue->crnt->tok);
}

/**
 * @brief After a rule is parsed, this function sets the head of the token
 * queue to the first unused token. All of the tokens that have been used
 * are discarded and collected by the garbage collector.
 */
void finalize_token_queue() {

    ENTER;

    TokQueue* tqueue = peek_link_list(tqueue_stack);
    tqueue->head     = tqueue->crnt;

    RET;
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

    ENTER;
    TokQueue* tqueue = peek_link_list(tqueue_stack);
    assert(tqueue != NULL);
    RETV((void*)tqueue->crnt);
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

    ENTER;

    TokQueue* tqueue = peek_link_list(tqueue_stack);
    assert(tqueue != NULL);

    tqueue->crnt = (TokQueueItem*)post;

    RET;
}

void dump_token_queue() {

    TokQueue* tqueue = peek_link_list(tqueue_stack);
    assert(tqueue != NULL);
    assert(tqueue->head != NULL);

    // Token* tok;
    // for(tok = iterate_token_queue(&mark); tok != NULL; tok = iterate_token_queue(&mark)) {
    for(TokQueueItem* crnt = tqueue->head; crnt != NULL; crnt = crnt->next) {
        print_terminal(crnt->tok);
        fputc('\n', stdout);
    }
}
