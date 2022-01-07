#ifndef SCANNER_H
#define SCANNER_H

typedef enum {
    END_OF_INPUT = 260, // End of input character.
    END_OF_FILE = 265,  // End of file character.
    WHITESPACE = 270,   // Any whitespace character.
} scanner_tokens;

typedef enum {
    CT_INVALID,
    CT_WHITESP,
    CT_DIGIT,
    CT_PUNCT,
    CT_SYMBOL,
} character_type;
extern character_type char_type_table[256];

// Return the char type from the character_type enum. Should do value check.
#define CHAR_TYPE(c)    (char_type_table[(int)(c)])
#define IS_SYM(c)       (CHAR_TYPE(c) == CT_SYMBOL)
#define IS_WHITE(c)     (CHAR_TYPE(c) == CT_WHITESP)
#define IS_PUNCT(c)     (CHAR_TYPE(c) == CT_PUNCT)
#define IS_DIGIT(c)     (CHAR_TYPE(c) == CT_DIGIT)

void init_scanner();
void scanner_open(const char* fname);
int get_line_no();
int get_col_no();
const char* get_fname();
int skip_whitespace();
int get_char();
int next_char();

#endif
