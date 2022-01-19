/*
 */
#include "common.h"


// Scanner data structure is private.
typedef struct _scanner {
    const char* fname;
    int line_no;
    int col_no;
    FILE* fp;

    struct _scanner* next;
} _scanner;

character_type char_type_table[256];

// Private data structure. There is only one scanner ever instantiated at any
// given time.
static _scanner* scanner = NULL;
static int current_char = 0;

// get the number of elelemnts in a data array by dividing the size of the
// array by the size of the type.
#define GET_SIZE(t, a)  ((int)(sizeof(a)/sizeof(t)))

static void close_file() {

    if(scanner != NULL) {
        fclose(scanner->fp);
        _free((void*)scanner->fname);
        struct _scanner* tpt = scanner;
        // pop the stack
        scanner = scanner->next;    // could be NULL;
        _free(tpt);
    }

    // else do nothing
}

/*
 * Simply return the current character.
 */
int get_char() {

    if(scanner != NULL)
        return current_char;
    else
        return END_OF_INPUT;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
/*
 * Dispose of the current character and read the next character from the stream,
 * making it the current character. Returns the current character. Could return
 * end of file or end of input. No attempt is made to interpret whitespace or
 * comments.
 */
int next_char() {

    if(scanner != NULL) {
        if(current_char == '\n') {
            scanner->line_no++;
            scanner->col_no = 1;
        }
        else
            scanner->col_no++;

        int ch = fgetc(scanner->fp);
        switch(ch) {
            case '\n':
            case ' ':
            case '\t':
            case '\r':
            case '\f':
            case '\v':
                current_char = ch;
                break;

            case EOF:
                // If EOF is read twice, then end of input has been reached.
                if(current_char == END_OF_FILE) {
                    current_char = END_OF_INPUT;
                }
                else {
                    close_file();
                    current_char = END_OF_FILE;
                }
                break;

            default:
                // a normal character.
                current_char = ch;
                break;
        }
    }
    else {
        // if the scanner data structure is NULL, then return end of input.
        current_char = END_OF_INPUT;
    }

    return current_char;
}
#pragma GCC diagnostic pop

/*
 * When this is entered, the "##" has already been read. Advanse the stream
 * until another "##" is seen.
 */
static void eat_multiline() {

    int finished = 0;

    MSG("ignoring multi-line comment");
    while(!finished) {
        next_char();
        if(current_char == '#') {
            next_char();
            if(current_char == '#') {
                next_char();
                finished++;
            }
        }
    }
}

/*
 * A single "#" marks a single line comment.
 */
static void eat_singleline() {

    MSG("ignoring single-line comment");
    while(current_char != '\n')
        next_char();
}

/*******************************************************************************
 * Public interface below this point.
 *
 */

/*
 * Skip whitespace characters and return the first character found that is not
 * a whitespace character. Advances the current character. Also skip comments.
 */
int skip_whitespace() {

    int finished = 0;

    while(!finished) {
        //while(current_char == WHITESPACE) {
        while(IS_WHITE(current_char)) {
            next_char();
        }

        // handle comments as white space.
        if(scanner != NULL) {
            // Single line comment goes until the end of the line. Multi-line
            // comments are limited by "##".
            if(current_char == '#') {
                next_char();
                if(current_char == '#')
                    eat_multiline();
                else
                    eat_singleline();
            }
            else if(CHAR_TYPE(current_char) == CT_INVALID) {
                // ignore invalid character
                warning("invalid character seen: %c", current_char);
                next_char();
                finished++;
            }
            else
                finished++;
        }
        else
            finished++;
    }

    return current_char;
}

/*
 * Initialize the scaner character type table.
 */
void init_scanner() {

    int white_chrs[] = {' ', '\t', '\f', '\v', '\r', '\n'};
    int punct_chrs[] = {'&', '%', '&', '*', '(', ')', '-', '+', '=', '[', ']',
                        '{', '}', '|', '\'', '\"', ',', '.', '/', '<', '>', '!'};
    // Note the '.'. Experemental.
    int symb_chars[] = {'`', '@', '$', '^', '?', '\\', '_', ':', ';', '.'};

    for(int i = 0; i < GET_SIZE(character_type, char_type_table); i++)
        char_type_table[i] = CT_INVALID;

    for(int i = 0; i < GET_SIZE(int, white_chrs); i++)
        char_type_table[white_chrs[i]] = CT_WHITESP;

    for(int i = 0; i < GET_SIZE(int, punct_chrs); i++)
        char_type_table[punct_chrs[i]] = CT_PUNCT;

    for(int i = '0'; i <= '9'; i++)
        char_type_table[i] = CT_DIGIT;

    for(int i = 0; i < GET_SIZE(int, symb_chars); i++)
        char_type_table[symb_chars[i]] = CT_SYMBOL;

    for(int i = 'a'; i <= 'z'; i++)
        char_type_table[i] = CT_SYMBOL;

    for(int i = 'A'; i <= 'Z'; i++)
        char_type_table[i] = CT_SYMBOL;
}

/*
 * Open a new scanner file and push it on to the scanner file stack. When the
 * last token is read from the file, it is automatically popped from the stack.
 */
void scanner_open(const char* fname) {

    _scanner* scn = _alloc(sizeof(_scanner));

    scn->fp = fopen(fname, "r");
    if(scn->fp == NULL) {
        fprintf(stderr, "cannot open file %s: %s\n", fname, strerror(errno));
        _free(scn);
        exit(1);
    }

    scn->line_no = 1;
    scn->col_no = 1;
    scn->fname = _strdup(fname);

    // push the stack
    if(scanner != NULL)
        scanner->next = scn;
    scanner = scn;

    // precharge the stream
    next_char();
}

/*
 * Return the line number where the token was read from.
 */
int get_line_no() {

    if(scanner != NULL)
        return scanner->line_no;
    else
        return -1;
}

/*
 * Return the column number at the end of the token.
 */
int get_col_no() {

    if(scanner!= NULL)
        return scanner->col_no;
    else
        return -1;
}

/*
 * Return the file name from which the token was read.
 */
const char* get_fname() {

    if(scanner != NULL)
        return scanner->fname;
    else
        return "no file open";
}
