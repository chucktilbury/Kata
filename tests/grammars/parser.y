%debug
%defines
%locations

%{
#include <stdio.h>
#include <stdbool.h>
#include "scanner.h"

FILE* outfile = NULL;
extern const char* file_name;

%}

%union {
    char* str;
    char* symbol;
    double num;
    int type;
    int scope;
    bool bval;
};

%token BREAK CASE CONTINUE CONST DEFAULT
%token DO ELSE FOR IF RETURN SWITCH IMPORT
%token NAMESPACE CLASS STRUCT WHILE START
%token IN TO AS YIELD EXIT INLINE
%token EQU NEQU LORE GORE OR AND
%token TRY EXCEPT RAISE CTOR DTOR
%token ADD SUB MUL DIV MOD

%token<type> NUMBER NOTHING STRING LIST DICT BOOLEAN PRINT TRACE TYPE
%token<scope> PUBLIC PRIVATE PROTECTED
%token<symbol> SYMBOL
%token<num> LITERAL_NUM
%token<str> LITERAL_STRG
%token<bval> LITERAL_BOOL

%define parse.error custom
%define parse.lac full
%locations

%left '@' UNARY
%left OR
%left AND
%left EQU NEQU
%left LORE GORE '<' '>'
%left '+' '-'
%left '*' '/' '%'
%right '^'

%%

module
    : module_item {}
    | module module_item {}
    ;

namespace_item
    : namespace_definition {}
    | class_definition {}
    | struct_definition {}
    | var_definition {}
    | scope_operator {}
    | func_definition {}
    ;

module_item
    : namespace_item
    | IMPORT formatted_strg {}
    | IMPORT formatted_strg AS SYMBOL {}
    | START func_body {}
    ;

scope_operator
    : PRIVATE {}
    | PUBLIC {}
    | PROTECTED {}
    ;

literal_type_name
    : NUMBER {}
    | STRING {}
    | BOOLEAN {}
    | NOTHING {}
    | LIST {}
    | DICT {}
    ;

    /* The compound reference must reference a type, not a data object */
type_name
    : literal_type_name {}
    | compound_reference {}
    ;

formatted_strg
    : LITERAL_STRG '(' expression_list ')' {}
    | LITERAL_STRG '(' ')' {}
    | LITERAL_STRG {}
    ;

literal_value
    : LITERAL_NUM {}
    | LITERAL_BOOL {}
    | formatted_strg {}
    ;

var_declaration
    : type_name SYMBOL {}
    | CONST type_name SYMBOL {}
    ;

var_declaration_list
    : var_declaration {}
    | var_declaration_list ',' var_declaration {}
    ;

var_definition
    : var_declaration {}
    | var_declaration '=' expression {}
    | var_declaration '=' list_init {}
    | var_declaration '=' dict_init {}
    ;

list_init_element
    : expression {}
    | dict_init {}
    ;

list_init_list
    : list_init_element {}
    | list_init_list ',' list_init_element {}
    ;

list_init
    : '[' list_init_list ']' {}
    ;

dict_init_element
    : LITERAL_STRG ':' expression {}
    | LITERAL_STRG ':' list_init {}
    | list_init {}
    ;

dict_init_list
    : dict_init_element {}
    | dict_init_list ',' dict_init_element {}
    ;

dict_init
    : '[' dict_init_list ']' {}
    | error {}
    ;

array_reference_param
    : '[' expression ']' {}
    | '[' array_reference_param ']' {}
    ;

array_param_list
    : array_reference_param {}
    | array_param_list array_reference_param {}
    ;

array_reference
    : SYMBOL array_param_list {}
    ;

function_reference_param
    : '(' expression_list ')' {}
    | '(' ')' {}
    ;

function_reference
    : SYMBOL function_reference_param {}
    ;

compound_reference_item
    : SYMBOL {}
    | array_reference {}
    | function_reference {}
    ;

compound_reference
    : compound_reference_item {}
    | compound_reference '.' compound_reference_item {}
    ;

expression_factor
    : literal_value {}
    | compound_reference {}
    ;

expression
    : expression_factor {}
    | expression '+' expression {}
    | expression '-' expression {}
    | expression '*' expression {}
    | expression '/' expression {}
    | expression '%' expression {}
    | expression EQU expression {}
    | expression NEQU expression {}
    | expression LORE expression {}
    | expression GORE expression {}
    | expression OR expression {}
    | expression AND expression {}
    | expression '<' expression {}
    | expression '>' expression {}
    | expression '^' expression {}
    | '-' expression %prec UNARY {}
    | '!' expression %prec UNARY {}
    | type_name '@' expression {}
    | '(' expression ')' {}
    ;

expression_list
    : expression {}
    | expression_list ',' expression {}
    | error {}
    ;

namespace_name
    : NAMESPACE SYMBOL {}
    | NAMESPACE scope_operator SYMBOL {}
    | error {}
    ;

namespace_list
    : namespace_item {}
    | namespace_list namespace_item {}
    ;

namespace_definition
    : namespace_name '{' namespace_list '}' {}
    | namespace_name '{' '}' {}
    ;

class_name
    : CLASS scope_operator SYMBOL {}
    | CLASS SYMBOL {}
    ;

    /* the compound reference must reference a type not a data object */
class_definition
    : class_name class_body {}
    | class_name '(' ')' class_body {}
    | class_name '(' compound_reference ')' class_body {}
    | error {}
    ;

    /* structs cannot have constructors and destructors */
struct_definition
    : STRUCT SYMBOL class_body {}
    ;

class_definition_element
    : var_declaration {}
    | func_declaration {}
    | func_definition {}
    | scope_operator {}
    | struct_definition {}
    | error {}
    ;

func_declaration
    : type_name SYMBOL '(' ')' {}
    | type_name SYMBOL '(' var_declaration_list ')' {}
    | CTOR '(' ')' {}
    | CTOR '(' var_declaration_list ')' {}
    | DTOR {}
    ;

class_body_list
    : class_definition_element {}
    | class_body_list class_definition_element {}
    ;

class_body
    : '{' '}' {}
    | '{' class_body_list '}' {}
    ;

func_definition
    : type_name SYMBOL '@' SYMBOL '(' ')' func_body {}
    | type_name SYMBOL '@' SYMBOL '(' var_declaration_list ')' func_body {}
    | SYMBOL '@' CTOR func_body {}
    | SYMBOL '@' CTOR '(' ')' func_body {}
    | SYMBOL '@' CTOR '(' var_declaration_list ')' func_body {}
    | SYMBOL '@' DTOR func_body {}
    | type_name SYMBOL '(' ')' func_body {}
    | type_name SYMBOL '(' var_declaration_list ')' func_body {}
    ;

func_body
    : '{' '}' {}
    | '{' func_body_element_list '}' {}
    ;

assignment
    : compound_reference '=' expression {}
    | compound_reference ADD expression {}
    | compound_reference SUB expression {}
    | compound_reference MUL expression {}
    | compound_reference DIV expression {}
    | compound_reference MOD expression {}
    ;

func_body_element
    : var_definition {}
    | compound_reference {}
    | assignment {}
    | while_clause {}
    | do_clause {}
    | for_clause {}
    | if_clause {}
    | try_clause {}
    | switch_clause {}
    | BREAK {}
    | CONTINUE {}
    | TRACE {}
    | INLINE { /*printf("%s\n", yylval.str);*/ }
    | YIELD '(' compound_reference ')' {}
    | TYPE '(' compound_reference ')' {}
    | EXIT '(' expression ')'{}
    | PRINT '(' expression_list ')' {}
    | RETURN '(' expression ')' {}
    | RETURN '(' ')' {}
    | RETURN {}
    | RAISE '(' SYMBOL ',' formatted_strg ')' {}
    | error {}
    ;

func_body_element_list
    : func_body_element {}
    | func_body_element_list func_body_element {}
    ;

while_clause
    : WHILE '(' expression ')' func_body {}
    | WHILE '(' ')' func_body {}
    | WHILE func_body {}
    ;

do_clause
    : DO func_body WHILE '(' expression ')' {}
    | DO func_body WHILE '(' ')' {}
    | DO func_body WHILE {}
    ;

for_operand
    : SYMBOL {}
    | type_name SYMBOL {}
    ;

    /* compound reference must refer to a valid RHS variable object */
for_clause
    : FOR '(' for_operand IN expression ')' func_body {}
    | FOR '(' for_operand IN expression TO LITERAL_NUM ')' func_body {}
    | FOR '(' ')' func_body {}
    | FOR func_body {}
    ;

if_clause
    : IF '(' expression ')' func_body else_clause {}
    ;

else_clause_mid
    : ELSE '(' expression ')' func_body {}
    ;

else_clause_final
    : ELSE func_body {}
    | ELSE '(' ')' func_body {}
    ;

else_clause_list
    : else_clause_mid {}
    | else_clause_list else_clause_mid {}
    ;

else_clause
    : %empty {}
    | else_clause_list {}
    | else_clause_list else_clause_final {}
    | else_clause_final {}
    ;

try_clause
    : TRY func_body except_clause {}
    ;

except_clause_mid
    : EXCEPT '(' SYMBOL ')' func_body {}
    ;

except_clause_final
    : EXCEPT '(' ')' func_body {}
    | EXCEPT func_body {}
    ;

except_clause_list
    : except_clause_mid {}
    | except_clause_list except_clause_mid {}
    ;

except_clause
    : except_clause_list {}
    | except_clause_list except_clause_final {}
    | except_clause_final {}
    ;

switch_clause
    : SWITCH '(' expression ')' case_body {}
    ;

case_clause
    : CASE '(' literal_value ')' func_body {}
    ;

case_clause_list
    : case_clause {}
    | case_clause_list case_clause {}
    ;

default_clause
    : DEFAULT func_body {}
    ;

case_body
    : case_clause_list {}
    | case_clause_list default_clause {}
    ;

%%

static int yyreport_syntax_error (const yypcontext_t *ctx) {

    fprintf(stderr, ">>>>> ");

    int res = 0;
    if(get_line_no() >= 0)
        fprintf (stderr, "%s:%d:%d: syntax error", get_file_name(), get_line_no(), get_col_no());
    else
        fprintf (stderr, "syntax error");

    // Report the tokens expected at this point.
    {
        enum { TOKENMAX = 5 };
        yysymbol_kind_t expected[TOKENMAX];
        int n = yypcontext_expected_tokens (ctx, expected, TOKENMAX);
        if (n < 0)
            // Forward errors to yyparse.
            res = n;
        else
            for (int i = 0; i < n; ++i)
                fprintf (stderr, "%s %s", i == 0 ? ": expected" : " or", yysymbol_name (expected[i]));
    }

    // Report the unexpected token.
    {
        yysymbol_kind_t lookahead = yypcontext_token (ctx);
        if (lookahead != YYSYMBOL_YYEMPTY)
            fprintf (stderr, " before %s", yysymbol_name (lookahead));
    }

    fprintf (stderr, "\n");
    return res;
}

void yyerror(const char* s) {

    if(get_col_no() >= 0)
        fprintf(stderr, "%s:%d:%d: %s\n", get_file_name(), get_line_no(), get_col_no(), s);
    else
        fprintf(stderr, "%s\n", s);
    //error("%s", s);
}

const char* tokenToStr(int tok) {

    return yysymbol_name(YYTRANSLATE(tok));
}
