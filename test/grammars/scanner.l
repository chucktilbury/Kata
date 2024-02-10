%{
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "parser.h"

#define _alloc_ds(t)    malloc(sizeof(t))
#define _free(p)        free((void*)p)

const char* _copy_str(const char* str) {

    size_t len = strlen(str) + 1;
    char* buf = malloc(len);
    memcpy(buf, str, len);
    return buf;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"

void yyerror(const char* s);

typedef struct _file_stack_ {
    const char* fname;
    int line;
    int col;
    FILE* fptr;
    YY_BUFFER_STATE buffer;
    struct _file_stack_* next;
} FileStack;

#define MAX_INCL 15
FileStack* fstack = NULL;
int incl_depth = 0;

int bidx = 0;
char buffer[1024 * 64];
const char* file_name;

static void __append_char(char ch) {
    // TODO: bounds checking
    buffer[bidx] = ch;
    bidx++;
}

static void __append_str(const char* str) {
    // TODO: bounds checking
    strcpy(&buffer[bidx], str);
    bidx += strlen(str);
}

int yycolumn = 1;

/* This is executed before every action. */
#define YY_USER_ACTION                                                   \
  fstack->line = prev_yylineno; fstack->col = yycolumn;                  \
  if (yylineno == prev_yylineno) yycolumn += yyleng;                     \
  else {                                                                 \
    for (yycolumn = 1; yytext[yyleng - yycolumn] != '\n'; ++yycolumn) {} \
    prev_yylineno = yylineno;                                            \
  }

%}

/* state definitions */
%x SQUOTES
%x DQUOTES
%x INLINE_BLOCK
%x COMMENT
%option noinput nounput
%option yylineno
%option noyywrap

%%

   /* Any indented text before the first rule goes at the top of the lexer.  */
   // int start_line, start_column;
   int prev_yylineno = yylineno;

\n      {}
[ \t\r] {}

"break"     { return BREAK; }
"case"      { return CASE; }
"continue"  { return CONTINUE; }
"const"     { return CONST; }
"default"   { return DEFAULT; }
"import"    { return IMPORT; }
"do"        { return DO; }
"else"      { return ELSE; }
"for"       { return FOR; }
"if"        { return IF; }
"return"    { return RETURN; }
"switch"    { return SWITCH; }
"while"     { return WHILE; }
"in"        { return IN; }
"to"        { return TO; }
"as"        { return AS; }
"yield"     { return YIELD; }
"exit"      { return EXIT; }
"try"       { return TRY; }
"except"    { return EXCEPT; }
"raise"     { return RAISE; }
"create"    { return CTOR; }
"destroy"   { return DTOR; }
"start"     { return START; }

"namespace" { return NAMESPACE; }
"class"     { return CLASS; }
"struct"    { return STRUCT; }

"public"    { yylval.scope = PUBLIC; return PUBLIC; }
"private"   { yylval.scope = PRIVATE; return PRIVATE; }
"protected" { yylval.scope = PROTECTED; return PROTECTED; }

"num"       { yylval.type = NUMBER; return NUMBER; }
"number"    { yylval.type = NUMBER; return NUMBER; }
"nothing"   { yylval.type = NOTHING; return NOTHING; }
"noth"      { yylval.type = NOTHING; return NOTHING; }
"string"    { yylval.type = STRING; return STRING; }
"strg"      { yylval.type = STRING; return STRING; }
"bool"      { yylval.type = BOOLEAN; return BOOLEAN; }
"boolean"   { yylval.type = BOOLEAN; return BOOLEAN; }
"list"      { yylval.type = LIST; return LIST; }
"dict"      { yylval.type = DICT; return DICT; }
"trace"     { yylval.type = TRACE; return TRACE; }
"print"     { yylval.type = PRINT; return PRINT; }
"type"      { yylval.type = TYPE; return TYPE; }

"true"      { yylval.bval = true; return LITERAL_BOOL; }
"false"     { yylval.bval = false; return LITERAL_BOOL; }
"on"        { yylval.bval = true; return LITERAL_BOOL; }
"off"       { yylval.bval = false; return LITERAL_BOOL; }

"<=" { return LORE; }
">=" { return GORE; }
"==" { return EQU; }
"!=" { return NEQU; }
"||" { return OR; }
"or" { return OR; }
"&&" { return AND; }
"and" { return AND; }

"+=" { return ADD; }
"-=" { return SUB; }
"*=" { return MUL; }
"/=" { return DIV; }
"%=" { return MOD; }

"+" { return '+'; }
"-" { return '-'; }
"=" { return '='; }
"/" { return '/'; }
"*" { return '*'; }
"%" { return '%'; }
"(" { return '('; }
")" { return ')'; }
"{" { return '{'; }
"}" { return '}'; }
"," { return ','; }
"." { return '.'; }
"<" { return '<'; }
">" { return '>'; }
":" { return ':'; }
"|" { return '|'; }
"[" { return '['; }
"]" { return ']'; }
"^" { return '^'; }
"@" { return '@'; }

"inline"[ \n\r\t]*"{"    { 
        bidx = 0;
        memset(buffer, 0, sizeof(buffer));
        BEGIN(INLINE_BLOCK);
    }

<INLINE_BLOCK>[^\}\n]*  { __append_str(yytext); }
<INLINE_BLOCK>\n     { __append_char('\n'); }
<INLINE_BLOCK>"}" {
        yylval.str = (char*)_copy_str(buffer);
        BEGIN(INITIAL);
        return INLINE;
    }

[a-zA-Z_][a-zA-Z_0-9]* {
        yylval.symbol = (char*)strdup(yytext);
        return SYMBOL;
    }

[0-9]+ {
        yylval.num = (double)strtol(yytext, NULL, 10);
        return LITERAL_NUM;
    }

    /* recognize a float */
([0-9]*\.)?[0-9]+([Ee][-+]?[0-9]+)? {
        yylval.num = strtod(yytext, NULL);
        return LITERAL_NUM;
    }

    /* double quoted strings have escapes managed */
\"  {
        bidx = 0;
        memset(buffer, 0, sizeof(buffer));
        BEGIN(DQUOTES);
    }

<DQUOTES>\" {
        yylval.str = (char*)_copy_str(buffer);
        BEGIN(INITIAL);
        return LITERAL_STRG;
    }

    /* the short rule matches before the long one does */
<DQUOTES>\\n { __append_char('\n'); }
<DQUOTES>\\r { __append_char('\r'); }
<DQUOTES>\\e { __append_char('\x1b'); }
<DQUOTES>\\t { __append_char('\t'); }
<DQUOTES>\\b { __append_char('\b'); }
<DQUOTES>\\f { __append_char('\f'); }
<DQUOTES>\\v { __append_char('\v'); }
<DQUOTES>\\\\ { __append_char('\\'); }
<DQUOTES>\\\" { __append_char('\"'); }
<DQUOTES>\\\' { __append_char('\''); }
<DQUOTES>\\\? { __append_char('\?'); }
<DQUOTES>\\.  { __append_char(yytext[1]); }
<DQUOTES>\\[0-7]{1,3} { __append_char((char)strtol(yytext + 1, 0, 8)); }
<DQUOTES>\\[xX][0-9a-fA-F]{1,3} { __append_char((char)strtol(yytext + 2, 0, 16)); }
<DQUOTES>[^\\\"\n]*  { __append_str(yytext); }
<DQUOTES>\n     { } /* track line numbers, but strip new line */

    /* single quoted strings are absolute literals */
\'  {
        bidx = 0;
        memset(buffer, 0, sizeof(buffer));
        BEGIN(SQUOTES);
    }

<SQUOTES>\' {
        yylval.str = (char*)strdup(buffer);
        BEGIN(INITIAL);
        return LITERAL_STRG;
    }

<SQUOTES>[^\\'\n]*  { __append_str(yytext); }
<SQUOTES>\\.    { __append_str(yytext); }
<SQUOTES>\n     { __append_str(yytext); } /* don't strip new lines */

    /* comments: If there is no newline at the end of the file, and the last 
       thing is a comment, then this is required to prevent a warning or a 
       syntax error. */
";" { BEGIN(COMMENT); }
<COMMENT>[^\n]* {}
<COMMENT><<EOF>> { BEGIN(INITIAL); }
<COMMENT>\n { BEGIN(INITIAL); /* TODO: support winders and whack */ }

.   {
        printf("Warning: unrecognized character: %c (0x%02X)\n", yytext[0], yytext[0]);
    }

<<EOF>> {

        incl_depth--;

        FileStack* tmp = fstack;
        _free(tmp->fname);
        fclose(tmp->fptr);
        _free(tmp);
        fstack = fstack->next;

        if(fstack == NULL) {
            yyterminate();
        }
        else {
            yy_delete_buffer(YY_CURRENT_BUFFER);
            yy_switch_to_buffer(fstack->buffer);
        }
    }


%%

int open_file(const char *fname) {

    if(incl_depth > MAX_INCL) {
        fprintf(stderr, "FATAL ERROR: Maximum include depth exceeded\n");
        exit(1);
    }
    incl_depth++;

    FileStack* fs = _alloc_ds(FileStack);
    fs->fname = _copy_str(fname);
    fs->line = 1;
    fs->col = 1;
    fs->next = NULL;

    // file_name = _copy_str(fname);
    yyin = fopen(fname, "r");
    if(yyin == NULL) {
        fprintf(stderr, "fatal error: cannot open input file: %s: %s\n", fname,
                strerror(errno));
        exit(1);
    }
    fs->fptr = yyin;
    fs->buffer = yy_create_buffer(yyin, YY_BUF_SIZE);
    yy_switch_to_buffer(fs->buffer);

    if(fstack != NULL) {
        fs->next = fstack;
        fstack = fs;
    }
    else
        fstack = fs;

    BEGIN(INITIAL);
    return 0;
}

int get_line_no() {

    if(fstack != NULL)
        return fstack->line;
    else
        return -1;
}

int get_col_no() {

    if(fstack != NULL)
        return fstack->col;
    else
        return -1;
}

const char* get_file_name() {

    if(fstack != NULL)
        return fstack->fname;
    else
        return "no file open";
}
