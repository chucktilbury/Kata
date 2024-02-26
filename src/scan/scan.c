/**
 * @file scanner.c
 *
 * @brief This is the implemetation of the default example scanner. It
 * implements scanning for keywords that are defined in the grammar and
 * also default constructed tokens that are literal numbers and literal
 * strings. Also symbols and operators such as "!=" and "==". It is
 * designed to be easy to change to match different requirements. The
 * scanner that is implemented here is intended to scan the tokens as they
 * are defined for the Simple programming language. See the header file for
 * more information.
 *
 * In addition to the operators and keywords (which are defined as strings
 * in the grammar), floating point numbers, symbols, and quoted strings are
 * tokenized from the input.
 *
 * @author Chuck Tilbury (chucktilbury@gmail.com)
 * @version 0.0
 * @date 01-07-2024
 * @copyright Copyright (c) 2024
 */
#include "scan.h"
//#include "keyword.h"
#include "tokens.h"

static Token token;

/**
 * @brief Comments are not retuned by the scanner. This reads from the ';'
 * and discards the data.
 */
static void eat_comment() {

    int ch;

    // the current char is a ';' when this is entered
    while(true) {
        ch = consume_char();
        if(ch == '\n' || ch == '\r' || ch == END_OF_FILE || ch == END_OF_INPUT)
            break;
    }
}

/**
 * @brief Scan a number in hex format. When this is called, the 'x' is the
 * current character.
 */
static void scan_unsigned() {

    int ch = get_char();

    while(true) {
        if(isxdigit(ch)) {
            add_string_char(token.str, ch);
            ch = consume_char();
        }
        else if(isspace(ch) || ispunct(ch) || ch == EOF)
            return;
        else {
            token.type = TOK_ERROR;
            clear_string(token.str);
            add_string_str(token.str, ": malformed hex number, expected hex digit, space or operator");
            return;
        }
    }
}

/**
 * @brief Scan the exponent of a floating point number. When this is
 * entered, the 'e' has already been seen but not consumed.
 */
static void scan_exponent() {

    // save the 'e'
    int ch = get_char();
    add_string_char(token.str, ch);
    ch = consume_char();

    // check for a sign.
    if(ch == '+' || ch == '-') {
        add_string_char(token.str, ch);
        ch = consume_char();
    }

    if(isdigit(ch)) {
        do {
            add_string_char(token.str, ch);
            ch = consume_char();
        } while(isdigit(ch));

        // when we run out of digits we are done if ending on white space
        // or an operator. Otherwise, it's an error.
        if(isspace(ch) || ispunct(ch) || ch == EOF)
            token.type = TOK_LITERAL_FLOAT;
        else {
            token.type = TOK_ERROR;
            add_string_str(token.str, ": malformed number: expected digits, space, or operator");
        }
    }
    else {
        token.type = TOK_ERROR;
        add_string_str(token.str, ": malformed number: expected digits");
    }
}

/**
 * @brief Scan the mantissa of a floating point number. When this is
 * entered, the '.' has been seen but not consumed.
 */
static void scan_mantissa() {

    int ch = get_char();
    add_string_char(token.str, ch);
    ch = consume_char();

    while(isdigit(ch)) {
        add_string_char(token.str, ch);
        ch = consume_char();
    }

    if(ch == 'e' || ch == 'E') {
        scan_exponent();
    }
    else if(isspace(ch) || ispunct(ch) || ch == EOF) {
        // no exponent. finished.
        token.type = TOK_LITERAL_FLOAT;
    }
    else {
        token.type = TOK_ERROR;
        add_string_str(token.str, ": malformed number: expected digits, exponent, space, or operator");
    }
}

/**
 * @brief All numbers in Simple are floating point with an optional
 * mantissa and exponent. Any string that strtod() recognizes is valid. If
 * the first character is a '0' then it must be followed by a '.' or a
 * non-digit. Leading '0' in a number is a syntax error.
 */
static void scan_number() {

    int ch = get_char();

    if(ch == '0') {
        // if the leading character is a zero
        ch = consume_char();
        add_string_char(token.str, '0');

        switch(ch) {
            case '.':
                // has the format of "0.xxx"
                // do not consume the dot.
                token.type = TOK_LITERAL_FLOAT;
                scan_mantissa();
                return;
            case 'x':
            case 'X':
                // scan for a hex number
                // do not consume the 'x'
                token.type = TOK_LITERAL_UNSIGNED;
                scan_unsigned();
                return;
            default:
                if(isspace(ch) || ispunct(ch)) {
                    // single zero with no other chars
                    token.type = TOK_LITERAL_SIGNED;
                    return;
                }
                else if(isdigit(ch)) {
                    token.type = TOK_ERROR;
                    add_string_str(token.str, ": malformed number: octal format not supported");
                    return;
                }
                // this should never happen
                token.type = TOK_ERROR;
                add_string_str(token.str, ": malformed number: unknown issue");
                return;
        }
    }
    else {
        // the first character was not a zero.
        bool finished = false;

        while(!finished) {
            ch = get_char();
            switch(ch) {
                case 'e':
                case 'E':
                    scan_exponent();
                    finished = true;
                    break;
                case '.':
                    scan_mantissa();
                    finished = true;
                    break;
                default:
                    if(isdigit(ch)) {
                        add_string_char(token.str, ch);
                        consume_char();
                    }
                    else if(isspace(ch) || ispunct(ch) || ch == EOF) {
                        token.type = TOK_LITERAL_SIGNED;
                        finished   = true;
                    }
                    else {
                        token.type = TOK_ERROR;
                        add_string_str(token.str, ": malformed number: expected digits, space, or operator");
                    }
            }
        }
    }
}

/**
 * @brief Single and multi-character operators as defined in the grammar.
 */
static void scan_operator() {

    int ch = get_char();

    // when this is entered, the current char is the first char of the operator
    switch(ch) {
        // single character operators
        case '^':
            token.type = TOK_CARAT;
            add_string_char(token.str, ch);
            consume_char();
            return;
        case '(':
            token.type = TOK_OPAREN;
            add_string_char(token.str, ch);
            consume_char();
            return;
        case ')':
            token.type = TOK_CPAREN;
            add_string_char(token.str, ch);
            consume_char();
            return;
        case '{':
            token.type = TOK_OCBRACE;
            add_string_char(token.str, ch);
            consume_char();
            return;
        case '}':
            token.type = TOK_CCBRACE;
            add_string_char(token.str, ch);
            consume_char();
            return;
        case '[':
            token.type = TOK_OSBRACE;
            add_string_char(token.str, ch);
            consume_char();
            return;
        case ']':
            token.type = TOK_CSBRACE;
            add_string_char(token.str, ch);
            consume_char();
            return;
        case '.':
            token.type = TOK_DOT;
            add_string_char(token.str, ch);
            consume_char();
            return;
        case ',':
            token.type = TOK_COMMA;
            add_string_char(token.str, ch);
            consume_char();
            return;
        case '@':
            token.type = TOK_AMPER;
            add_string_char(token.str, ch);
            consume_char();
            return;
        case ':':
            token.type = TOK_COLON;
            add_string_char(token.str, ch);
            consume_char();
            return;
        case '&':
            token.type = TOK_AND;
            add_string_char(token.str, ch);
            consume_char();
            return;
        case '|':
            token.type = TOK_OR;
            add_string_char(token.str, ch);
            consume_char();
            return;

        // multi-character operators
        case '<': {
            add_string_char(token.str, ch);
            ch = consume_char();
            if(ch == '=') {
                add_string_char(token.str, ch);
                consume_char();
                token.type = TOK_LORE;
            }
            else
                token.type = TOK_OPAREN;
        }
            return;
        case '>': {
            add_string_char(token.str, ch);
            ch = consume_char();
            if(ch == '=') {
                add_string_char(token.str, ch);
                consume_char();
                token.type = TOK_GORE;
            }
            else
                token.type = TOK_CPAREN;
        }
            return;
        case '=': {
            add_string_char(token.str, ch);
            ch = consume_char();
            if(ch == '=') {
                add_string_char(token.str, ch);
                consume_char();
                token.type = TOK_EQU;
            }
            else
                token.type = TOK_ASSIGN;
        }
            return;
        case '+': {
            add_string_char(token.str, ch);
            ch = consume_char();
            if(ch == '=') {
                add_string_char(token.str, ch);
                consume_char();
                token.type = TOK_ADD_ASSIGN;
            }
            else
                token.type = TOK_ADD;
        }
            return;
        case '-': {
            add_string_char(token.str, ch);
            ch = consume_char();
            if(ch == '=') {
                add_string_char(token.str, ch);
                consume_char();
                token.type = TOK_SUB_ASSIGN;
            }
            else
                token.type = TOK_SUB;
        }
            return;
        case '*': {
            add_string_char(token.str, ch);
            ch = consume_char();
            if(ch == '=') {
                add_string_char(token.str, ch);
                consume_char();
                token.type = TOK_MUL_ASSIGN;
            }
            else
                token.type = TOK_MUL;
        }
            return;
        case '/': {
            add_string_char(token.str, ch);
            ch = consume_char();
            if(ch == '=') {
                add_string_char(token.str, ch);
                consume_char();
                token.type = TOK_DIV_ASSIGN;
            }
            else
                token.type = TOK_DIV;
        }
            return;
        case '%': {
            add_string_char(token.str, ch);
            ch = consume_char();
            if(ch == '=') {
                add_string_char(token.str, ch);
                consume_char();
                token.type = TOK_MOD_ASSIGN;
            }
            else
                token.type = TOK_MOD;
        }
            return;
        case '!': {
            add_string_char(token.str, ch);
            ch = consume_char();
            if(ch == '=') {
                add_string_char(token.str, ch);
                consume_char();
                token.type = TOK_NEQU;
            }
            else
                token.type = TOK_NOT;
        }
            return;
    }
}

/**
 * @brief Check to see if the word is a keyword using a binary search of
 * the keyword list that is generated by the parser generator. Note that
 * the keyword list is sorted by the parser generator.
 */
static void check_keyword() {

    const char* s = raw_string(token.str);
    int l = 0, r = num_keywords - 1, m, x;

    // setup the token. If the string is not found, then this is not changed.
    token.type = TOK_SYMBOL;

    // the loop will run till there are elements in the
    // subarray as l > r means that there are no elements to
    // consider in the given subarray
    while(l <= r) {
        // calculating mid point
        m = l + (r - l) / 2;
        // see how the string compares.
        x = strcasecmp(keyword_list[m].str, s);

        // Check if x is present at mid
        if(x == 0) {
            token.type = keyword_list[m].type;
            break;
        }
        // If x greater than zero ignore left half
        if(x < 0) {
            l = m + 1;
        }
        // If x is smaller than m, ignore right half
        else {
            r = m - 1;
        }
    }
    // if we reach here, then element was not present so return it as
    // a SYMBOL.
}

/**
 * @brief Scan an inline block. This is a special block that can contain any
 * character. It is introduced with a '{' character and ends with '}'. The
 * text between them almost certainly will contain curly braces, so a count
 * of them is maintained and when the count is zero, then the loop exits. If
 * the leading '{' is not located, then a syntax error must be returned. When
 * this is entered, the token string has to be reset. Only the inline text is
 * returned without the inline keyword and without the enclosing {}.
 */
static void scan_inline_block() {

    // skip white space
    while(isspace(consume_char()))
        ;

    // get the opening '{'
    if(get_char() != '{') {
        // Do not consume the character in the presumption that the the
        // error handler will want to know what it was.
        token.type = TOK_ERROR;
        add_string_str(token.str, ": expected a '{' to introduce the block");
        return;
    }

    int count = 1;
    int ch;

    clear_string(token.str);
    // consume the opening '{'
    consume_char();
    // copy the characters
    do {
        ch = get_char();
        if(ch == '{') {
            count++;
        }
        else if(ch == '}') {
            count--;
        }
        add_string_char(token.str, ch);
        consume_char();
    } while(count > 0 && ch != EOF);

    token.type = TOK_INLINE;
}

/**
 * @brief A word is defined as [a-zA-Z_][0-9a-zA-Z_]*. The word is first
 * read and then it is checked to see if it is a keyword. Keywords and not
 * case- sensitive.
 */
static void scan_word() {

    int ch;

    // current char is the first letter of the word when this is entered
    while(true) {
        ch = get_char();
        if(isalnum(ch) || ch == '_') {
            add_string_char(token.str, ch);
            consume_char();
        }
        else
            break;
    }

    // look for a keyword, or else a SYMBOL
    check_keyword();

    // This scanner supports an "inline" keyword as a separate entity. When
    // it is located, then all characters between the following {} pair are
    // copied as an inline block. The compiler then places this text
    // directly into the output without modifications.
    if(token.type == TOK_INLINE)
        scan_inline_block();
}

/**
 * @brief Scan a single quote string. All characters in a single quote
 * string are absolute literals. A single quote string cannot contain a
 * single quote. The string is returned as a LITERAL_STR without the
 * quotes. Note that strings may have newlines in it.
 */
static void scan_squote_str() {

    int ch;
    bool finished = false;

    consume_char(); // consume the '\''
    while(!finished) {
        ch = get_char();
        if(ch == '\'') {
            token.type = TOK_LITERAL_SSTR;
            consume_char();
            finished = true;
        }
        else if(ch == EOF) {
            token.type = TOK_ERROR;
            // the string could be very long, so clear it before returning.
            clear_string(token.str);
            add_string_str(token.str, "invalid string. Unexpected end of file.");
            finished = true;
        }
        else {
            add_string_char(token.str, ch);
            consume_char();
        }
    }
}

/**
 * @brief Scan a double quote string. Double quote strings support escape
 * characters using a backslash. The string is returned as a LITERAL_STR
 * without the quotes. All of the escape characters are translated and
 * inserted into the string.
 */
static void scan_dquote_str() {

    int ch        = consume_char(); // consume the '\"'
    bool finished = false;

    while(!finished) {
        ch = get_char();
        if(ch == '\"') {
            finished   = true;
            token.type = TOK_LITERAL_DSTR;
            consume_char();
        }
        else if(ch == '\\') {
            ch = consume_char();
            switch(ch) {
                case '\\':
                    add_string_char(token.str, '\\');
                    consume_char();
                    break;
                case 'n':
                    add_string_char(token.str, '\n');
                    consume_char();
                    break;
                case 'r':
                    add_string_char(token.str, '\r');
                    consume_char();
                    break;
                case '\'':
                    add_string_char(token.str, '\'');
                    consume_char();
                    break;
                case '\"':
                    add_string_char(token.str, '\"');
                    consume_char();
                    break;
                case 't':
                    add_string_char(token.str, '\t');
                    consume_char();
                    break;
                case 'f':
                    add_string_char(token.str, '\f');
                    consume_char();
                    break;
                case 'v':
                    add_string_char(token.str, '\v');
                    consume_char();
                    break;
                case 'a':
                    add_string_char(token.str, '\a');
                    consume_char();
                    break;
                case 'b':
                    add_string_char(token.str, '\b');
                    consume_char();
                    break;
                case 'e':
                    add_string_char(token.str, 0x1B);
                    consume_char();
                    break;
                case 'x': { // the next 2 characters must be hex digits
                    char buf[5] = "0x";
                    buf[2]      = consume_char();
                    buf[3]      = consume_char();
                    buf[4]      = 0;
                    if(isxdigit(buf[2]) && isxdigit(buf[3])) {
                        add_string_char(token.str, (int)strtol(buf, NULL, 16));
                    }
                    else {
                        // syntax error
                        token.type = TOK_ERROR;
                        clear_string(token.str);
                        add_string_fmt(token.str, "invalid string. expected a hex number but got \\x%c%c",
                                       buf[2], buf[3]);
                        finished = true;
                    }
                } break;
                default: // just store the next character, whatever it is
                    add_string_char(token.str, ch);
                    break;
            }
        }
        else if(ch == EOF) {
            token.type = TOK_ERROR;
            // the string could be very long, so clear it before returning.
            clear_string(token.str);
            add_string_str(token.str, "invalid string. Unexpected end of file.");
            finished = true;
        }
        else {
            add_string_char(token.str, ch);
            consume_char();
        }
    }
}

/**
 * @brief Mark the token with the file name and the line number.
 */
static void finish_token() {

    token.line_no = get_line_no();
    token.col_no  = get_col_no();
    const char* s = get_fname();
    if(s != NULL)
        token.fname = _DUP_STR(strrchr(s, '/') + 1);
    else
        token.fname = _DUP_STR("no open file");
}

/**
 * @brief Allocate the data structures that the scanner uses. This function
 * must be called before any characters are scanned.
 */
void init_scanner(const char* fname) {

    memset(&token, 0, sizeof(Token));
    token.str = create_string(NULL);

    open_file(fname);
}

/**
 * @brief This function reads a token from the input stream and returns in
 * in a pointer to a global static Token data structure. When the token is
 * read, a deep copy is performed to preserve the value. This keeps us from
 * having to recompose the data structhre every time a token is read.
 */
Token* scan_token() {

    bool finished = false;
    int ch;

    clear_string(token.str);
    while(!finished) {
        ch = get_char();

        // skip whitespace. line counting is done elsewhere
        if(isspace(ch)) {
            consume_char();
            continue;
        }
        // comment precursor
        else if(ch == ';') {
            eat_comment();
        }
        // single quote strings are absolute literals
        else if(ch == '\'') {
            scan_squote_str();
            finished = true;
        }
        // double quote strings can have escape characters
        else if(ch == '\"') {
            scan_dquote_str();
            finished = true;
        }
        // scan a number and return it
        else if(isdigit(ch)) {
            scan_number();
            finished = true;
        }
        // symbols and keywords start with a letter
        else if(isalpha(ch) || ch == '_') {
            scan_word();
            finished = true;
        }
        // scan an operator and return it
        else if(ispunct(ch)) {
            scan_operator();
            finished = true;
        }
        // end of input has been reached where there are no more files in
        // the file stack.
        else if(ch == END_OF_FILE) {
            token.type = TOK_END_OF_FILE;
            finished   = true;
        }
        else if(ch == END_OF_INPUT) {
            token.type = TOK_END_OF_INPUT;
            finished   = true;
        }
    }

    finish_token();
    return &token;
}

/**
 * @brief Print the content of the token for debugging.
 *
 * @param tok
 */
void print_token(Token* tok) {

    printf("%s: \"%s\": %d: %d: \"%s\"", tok_to_str(tok),
           raw_string(tok->str), tok->line_no, tok->col_no, tok->fname);
}

/*
 * Low level get token, outside of official interface, so that token can be
 * a static var.
 */
Token* sneak_token() {
    return &token;
}
