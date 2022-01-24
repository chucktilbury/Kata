// This file is automatically generated by make process
struct _keyword {
    const char* str;
    token_type type;
} _keyword_list[] = {
    {"and", AND_TOK},
    {"any", ANY_TOK},
    {"bool", BOOL_TOK},
    {"boolean", BOOL_TOK},
    {"break", BREAK_TOK},
    {"case", CASE_TOK},
    {"class", CLASS_TOK},
    {"construct", CONSTRUCT_TOK},
    {"continue", CONTINUE_TOK},
    {"default", DEFAULT_TOK},
    {"destroy", DESTROY_TOK},
    {"dict", DICT_TOK},
    {"dictionary", DICT_TOK},
    {"do", DO_TOK},
    {"else", ELSE_TOK},
    {"enter", ENTER_TOK},
    {"eq", EQUALITY_TOK},
    {"except", EXCEPT_TOK},
    {"float", FLOAT_TOK},
    {"for", FOR_TOK},
    {"gt", GREATER_TOK},
    {"gte", GTE_TOK},
    {"if", IF_TOK},
    {"import", IMPORT_TOK},
    {"int", INT_TOK},
    {"leave", LEAVE_TOK},
    {"list", LIST_TOK},
    {"lt", LESS_TOK},
    {"lte", LTE_TOK},
    {"neq", NOT_EQUAL_TOK},
    {"not", NOT_TOK},
    {"nothing", ANY_TOK},
    {"or", OR_TOK},
    {"priv", PRIVATE_TOK},
    {"private", PRIVATE_TOK},
    {"protected", PROTECTED_TOK},
    {"public", PUBLIC_TOK},
    {"raise", RAISE_TOK},
    {"return", RETURN_TOK},
    {"signed", INT_TOK},
    {"string", STRING_TOK},
    {"switch", SWITCH_TOK},
    {"throw", RAISE_TOK},
    {"try", TRY_TOK},
    {"uint", UINT_TOK},
    {"unsigned", UINT_TOK},
    {"while", WHILE_TOK},
};

#define KW_LIST_LEN ((sizeof(_keyword_list)/sizeof(struct _keyword))-1)

