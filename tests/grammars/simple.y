%debug
%defines
%locations

%{
#include <stdio.h>
#include <stdint.h>
#include "util.h"
#include "log.h"
#include "ast.h"
#include "scanner.h"

//ast_module_t* ast_module_root = NULL;
/*
static const char* strlst_raw(StrList* ptr) {

    Str* s = create_string(NULL);
    Str* tmp;

    reset_str_list(ptr);
    add_string_Str(s, iterate_str_list(ptr));
    while(NULL != (tmp = iterate_str_list(ptr))) {
        add_string_char(s, '.');
        add_string_Str(s, tmp);
    }

    return raw_string(s); // assumes using GC....
}
*/
%}

%union {
    StrList* sym;
    //Str* str;
    const char* str;
    double fnum;
    int64_t inum;
    uint64_t unum;
    int symbol;
    void* nterm;
};

%token NAMESPACE CLASS CREATE DESTROY
%token IF ELSE WHILE DO FOR IN TO TRY
%token EXCEPT RAISE RETURN EXIT SWITCH CASE YIELD TRACE PRINT IMPORT
%token TRUE FALSE BREAK CONTINUE INLINE
%token ENTRY DEFAULT DOT COMMA COLON AS
%token CPAREN OPAREN OBLOCK CBLOCK OBRACE CBRACE
    // COMPOUND
    //%token <sym> SYMBOL

%token <str> STRG_CONST SYMBOL
%token <fnum> FLOAT_CONST
%token <inum> INT_CONST
%token <unum> UNSIGNED_CONST
%token <symbol> PUBLIC PRIVATE PROTECTED INTEGER UNSIGNED
%token <symbol> FLOAT STRING DICT LIST BOOLEAN NOTHING
%token <symbol> ASSIGN ADD SUB MUL DIV MOD POW
%token <symbol> NOT OR AND EQU NEQU LORE GORE OPOINT CPOINT
%token <symbol> ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN

%type <sym> compound_name
%type <str> formatted_string

%define parse.error custom
%locations

%glr-parser
    // %expect-rr 1

%precedence CAST
%left OR
%left AND
%left EQU NEQU
%left LORE GORE OPOINT CPOINT
%left ADD SUB
%left MUL DIV MOD
%precedence UNARY
%right POW

%%
module
    : module_body {
            PTRACE("module");
        }
    ;

module_element
    : namespace_element {
            PTRACE("module_element:namespace_element");
        }
    | import_statement {
            PTRACE("module_element:import_statement");
        }
    | entry_definition {
            PTRACE("module_element:entry_definition");
        }
    ;

module_body
    : module_element {
            PTRACE("module_body:module_element");
        }
    | module_body module_element {
            PTRACE("module_body:module_body");
        }
    ;

entry_definition
    : ENTRY func_block {
            PTRACE("module_element:ENTRY");
        }
    ;

scope
    : PUBLIC {
            PTRACE("scope:PUBLIC");
        }
    | PRIVATE {
            PTRACE("scope:PRIVATE");
        }
    | PROTECTED {
            PTRACE("scope:PROTECTED");
        }
    ;

type_name
    : compound_name {
            PTRACE("type_name:compound_name: %s", raw_string(join_str_list($1, ".")));
        }
    | FLOAT {
            PTRACE("type_name:FLOAT");
        }
    | INTEGER {
            PTRACE("type_name:INTEGER");
        }
    | UNSIGNED {
            PTRACE("type_name:UNSIGNED");
        }
    | STRING {
            PTRACE("type_name:STRING");
        }
    | DICT {
            PTRACE("type_name:DICT");
        }
    | LIST {
            PTRACE("type_name:LIST");
        }
    | BOOLEAN {
            PTRACE("type_name:BOOLEAN");
        }
    | NOTHING {
            PTRACE("type_name:NOTHING");
        }
    ;

    /*
     * Compound name and compound reference create a reduce/reduce conflict.
     * This is resolved by defining the compound_reference first, since it has
     * the most possible combinations. If the compound name gets selected as
     * the reference, then no harm is done.
     */
compound_name
    : SYMBOL %dprec 1 {
            $$ = create_str_list();
            add_str_list($$, create_string($1));
            PTRACE("compound_name:create:%s", $1);
        }
    | compound_name DOT SYMBOL {
            add_str_list($$, create_string($3));
            PTRACE("compound_name:add:%s", $3);
        }
    ;

compound_reference
    : compound_reference_element {
            PTRACE("compound_reference:compound_reference_element");
        }
    | compound_reference DOT compound_reference_element {
            PTRACE("compound_reference:compound_reference.elem");
        }
    ;

compound_reference_element
    : SYMBOL %dprec 2{
            PTRACE("compound_reference_element:compound_reference:\"%s\"", $1);
        }
    | func_reference {
            PTRACE("compound_reference_element:func_reference");
        }
    | array_reference {
            PTRACE("compound_reference_element:array_reference");
        }
    ;

formatted_string
    : STRG_CONST {
            PTRACE("formatted_string:STRG_CONST: %s", $1);
            $$ = $1;
        }
    | STRG_CONST OPAREN CPAREN {
            PTRACE("formatted_string:STRG_CONST(): %s", $1);
            $$ = $1;
        }
    | STRG_CONST OPAREN expr_list CPAREN {
            PTRACE("formatted_string:STRG_CONST(expr_lst): %s", $1);
            $$ = $1;
        }
    ;

func_reference
    : compound_name OPAREN CPAREN {
            PTRACE("func_reference:%s()", raw_string(join_str_list($1, ".")));
        }
    | compound_name OPAREN expr_list CPAREN {
            PTRACE("func_reference:%s(expr_lst)", raw_string(join_str_list($1, ".")));
        }
    ;

array_reference
    : compound_name array_reference_list {
            PTRACE("array_reference:%s[expr]", raw_string(join_str_list($1, ".")));
        }
    ;

array_reference_list
    : OBRACE expression CBRACE {
            PTRACE("array_reference_list:create");
        }
    | array_reference_list OBRACE expression CBRACE {
            PTRACE("array_reference_list:add");
        }
    ;

    /*
     * Note that an import statment is not part of the AST, but causes
     * the imported code to the parsed inline to the current module.
     */
import_statement
    : IMPORT compound_name {
            PTRACE("import_statement:compound_name: %s", raw_string(join_str_list($2, ".")));
        }
    | IMPORT compound_name AS SYMBOL {
            PTRACE("import_statement:compound_name: %s AS %s",
                    raw_string(join_str_list($2, ".")), $4);
        }
    ;

namespace_definition
    : NAMESPACE SYMBOL OBLOCK namespace_block CBLOCK {
            PTRACE("namespace_definition:%s", $2);
        }
    ;

namespace_block
    : namespace_element {
            PTRACE("namespace_block:namespace_element");
        }
    | namespace_block namespace_element {
            PTRACE("namespace_block:namespace_block");
        }
    ;

namespace_element
    : namespace_definition {
            PTRACE("namespace_element:namespace_definition");
        }
    | class_definition {
            PTRACE("namespace_element:class_definition");
        }
    | func_definition {
            PTRACE("namespace_element:func_definition");
        }
    | ctor_definition {
            PTRACE("namespace_element:ctor_definition");
        }
    | dtor_definition {
            PTRACE("namespace_element:dtor_definition");
        }
    | var_declaration {
            PTRACE("namespace_element:var_declaration");
        }
    | scope {
            PTRACE("namespace_element:scope");
        }
    ;

class_definition
    : CLASS SYMBOL class_parameters class_block {
            PTRACE("class_definition:%s", $2);
        }
    ;

class_parameters
    : %empty {
            PTRACE("class_parameters:%%empty");
        }
    | OPAREN CPAREN {
            PTRACE("class_parameters:()");
        }
    | OPAREN compound_name CPAREN {
            PTRACE("class_parameters:(compound_name)");
        }
    ;

class_block
    : OBLOCK CBLOCK {
            PTRACE("class_block:{}");
        }
    | OBLOCK class_body CBLOCK {
            PTRACE("class_block:{body}");
        }
    ;

class_body
    : class_body_element {
            PTRACE("class_body:class_body_element");
        }
    | class_body class_body_element {
            PTRACE("class_body:class_body");
        }
    ;

class_body_element
    : var_declaration {
            PTRACE("class_body_element:var_declaration");
        }
    | func_declaration {
            PTRACE("class_body_element:func_declaration");
        }
    | scope {
            PTRACE("class_body_element:scope");
        }
    ;

var_declaration
    : type_name SYMBOL {
            PTRACE("var_declaration: \"%s\"", $2);
        }
    ;

func_declaration
    : type_name SYMBOL func_decl_parms {
            PTRACE("func_declaration: \"%s\"", $2);
        }
    | CREATE func_decl_parms {
            PTRACE("func_declaration:CREATE");
        }
    | DESTROY {
            PTRACE("func_declaration:DESTROY");
        }
    ;

func_decl_parms
    : OPAREN CPAREN {
            PTRACE("func_decl_parms()");
        }
    | OPAREN func_decl_parms_list CPAREN {
            PTRACE("func_decl_parms(func_decl_parms_list)");
        }
    ;

func_decl_parms_list
    : func_decl_parms_elem {
            PTRACE("func_decl_parms_list:func_decl_parms_elem");
        }
    | func_decl_parms_list COMMA func_decl_parms_elem {
            PTRACE("func_decl_parms_list:func_decl_parms_list");
        }
    ;

func_decl_parms_elem
    : type_name SYMBOL {
            PTRACE("func_decl_parms_elem:%s", $2);
        }
    ;

bool_literal
    : TRUE {
            PTRACE("bool_literal:TRUE");
        }
    | FALSE {
            PTRACE("bool_literal:FALSE");
        }
    ;

primary
    : compound_reference {
            PTRACE("primary:compound_reference");
        }
    | const_expression {
            PTRACE("primary:const_expression");
        }
    | OPAREN expression CPAREN {
            PTRACE("primary:(e)");
        }
    ;

const_expression
    : formatted_string {
            PTRACE("const_expression:formatted_string");
        }
    | bool_literal {
            PTRACE("const_expression:bool_literal");
        }
    | FLOAT_CONST {
            PTRACE("const_expression:%f", $1);
        }
    | INT_CONST {
            PTRACE("const_expression:%ld", $1);
        }
    | UNSIGNED_CONST {
            PTRACE("const_expression:%lu", $1);
        }
    ;

expression
    : primary {
            PTRACE("expression:primary");
        }
    | expression AND expression {
            PTRACE("expression:e and e");
        }
    | expression OR expression {
            PTRACE("expression:e or e");
        }
    | expression EQU expression {
            PTRACE("expression:e == e");
        }
    | expression NEQU expression {
            PTRACE("expression:e != e");
        }
    | expression LORE expression {
            PTRACE("expression:e <= e");
        }
    | expression GORE expression {
            PTRACE("expression:e >= e");
        }
    | expression OPOINT expression {
            PTRACE("expression:e < e");
        }
    | expression CPOINT expression {
            PTRACE("expression:e > ");
        }
    | expression ADD expression {
            PTRACE("expression:e + e");
        }
    | expression SUB expression {
            PTRACE("expression:e - e");
        }
    | expression MUL expression {
            PTRACE("expression:e * e");
        }
    | expression DIV expression {
            PTRACE("expression:e / e");
        }
    | expression MOD expression {
            PTRACE("expression:e %% e");
        }
    | expression POW expression {
            PTRACE("expression:e ^ e");
        }
    | NOT expression %prec UNARY {
            PTRACE("expression:! e");
        }
    | SUB expression %prec UNARY {
            PTRACE("expression:- e");
        }
    | cast_expr %prec CAST {
            PTRACE("expression:cast_expr");
        }
    ;

cast_expr
    : OPOINT type_name CPOINT expression {
            PTRACE("cast_expr:<type>expr");
        }
    ;

expr_list
    : expression {
            PTRACE("expr_list:expression");
        }
    | expr_list COMMA expression {
            PTRACE("expr_list:expr_list");
        }
    ;

member
    : SYMBOL COLON {
            PTRACE("member:%s", $1);
        }
    ;

func_definition
    : type_name member SYMBOL func_decl_parms func_block {
            PTRACE("func_definition:%s", $3);
        }
    ;

ctor_definition
    : member CREATE func_decl_parms func_block {
            PTRACE("ctor_definition");
        }
    ;

dtor_definition
    : member DESTROY func_block {
            PTRACE("dtor_definition");
        }
    ;

func_block
    : OBLOCK CBLOCK {
            PTRACE("func_block{}");
        }
    | OBLOCK func_content CBLOCK {
            PTRACE("func_block{func_content}");
        }
    ;

assignment_oper
    : ASSIGN {
            PTRACE("assignment_oper:=");
        }
    | ADD_ASSIGN {
            PTRACE("assignment_oper:+=");
        }
    | SUB_ASSIGN {
            PTRACE("assignment_oper:-=");
        }
    | MUL_ASSIGN {
            PTRACE("assignment_oper:*=");
        }
    | DIV_ASSIGN {
            PTRACE("assignment_oper:/=");
        }
    | MOD_ASSIGN {
            PTRACE("assignment_oper:%%=");
        }
    ;

assignment
    : assignment_oper expression {
            PTRACE("assignment");
        }
    ;

data_definition
    : var_declaration {
            PTRACE("data_definition");
        }
    | var_declaration assignment {
            PTRACE("data_definition:assignment");
        }
    ;

func_content
    : func_content_elem {
            PTRACE("func_content:func_content_elem");
        }
    | func_content func_content_elem {
            PTRACE("func_content:func_content");
        }
    ;

    /*
     * The 3 rules data_definition, compound_reference, and compound_reference
     * assignment produce a reduce/reduce conflict, which is because they
     * all fight over who gets to use the SYMBOL. However, it seems to do the
     * right thing, and there is no way to really fix it that I have seen, so
     * I am going to leave it for now and work other things.
     */
func_content_elem
    : func_block {
            PTRACE("func_content:func_block");
        }
    | data_definition {
            PTRACE("func_content:data_definition");
        }
    | compound_reference {
            PTRACE("func_content:func_reference");
            /* Verify that the compound_reference referrs to a
                usable function call. */
        }
    | compound_reference assignment {
            PTRACE("func_content_elem:compound_reference:assignment");
            /* Verify that the compound_reference referrs to a
                usable left hand object. */
        }
    | if_else_clause {
            PTRACE("func_content:if_else_clause");
        }
    | while_clause {
            PTRACE("func_content:while_clause");
        }
    | do_clause {
            PTRACE("func_content:do_clause");
        }
    | for_clause {
            PTRACE("func_content:for_clause");
        }
    | switch_case_clause {
            PTRACE("func_content:switch_case_clause");
        }
    | try_except_clause {
            PTRACE("func_content:try_except_clause");
        }
    | return_statement {
            PTRACE("func_content:return_statement");
        }
    | raise_statement {
            PTRACE("func_content:raise_statement");
        }
    | exit_statement {
            PTRACE("func_content:exit_statement");
        }
    | yield_statement {
            PTRACE("func_content:yield_statement");
        }
    | trace_statement {
            PTRACE("func_content:trace_statement");
        }
    | print_statement {
            PTRACE("func_content:print_statement");
        }
    | break_statement {
            PTRACE("func_content:break_statement");
        }
    | cont_statement {
            PTRACE("func_content:cont_statement");
        }
    | inline_block {
            PTRACE("func_content:inline block");
        }
    ;

inline_block
    : INLINE formatted_string {
            PTRACE("inline_block:%s", $2);
        }
    ;

print_statement
    : PRINT {
            PTRACE("print_statement");
        }
    | PRINT OPAREN CPAREN {
            PTRACE("print_statement()");
        }
    | PRINT OPAREN formatted_string CPAREN {
            PTRACE("print_statement(str)");
        }
    ;

break_statement
    : BREAK {
            PTRACE("break_statement");
        }
    ;

cont_statement
    : CONTINUE {
            PTRACE("cont_statement");
        }
    ;

trace_statement
    : TRACE {
            PTRACE("trace_statement");
        }
    | TRACE OPAREN CPAREN {
            PTRACE("trace_statement()");
        }
    | TRACE OPAREN formatted_string CPAREN {
            PTRACE("trace_statement(str)");
        }
    ;

yield_statement
    : YIELD OPAREN SYMBOL CPAREN {
            PTRACE("yield_statement:%s", $3);
        }
    ;

exit_statement
    : EXIT OPAREN expression CPAREN {
            PTRACE("exit_statement");
        }
    ;

return_statement
    : RETURN {
            PTRACE("return_statement");
        }
    | RETURN OPAREN CPAREN {
            PTRACE("return_statement()");
        }
    | RETURN OPAREN expression CPAREN {
            PTRACE("return_statement(expr)");
        }
    ;

raise_statement
    : RAISE OPAREN compound_name CPAREN {
            PTRACE("raise_statement");
        }
    ;

if_else_clause
    : if_clause {
            PTRACE("if_else_clause:");
        }
    | if_clause else_clause_list {
            PTRACE("if_else_clause:else_clause_list");
        }
    | if_clause else_clause_list else_clause_final {
            PTRACE("if_else_clause:else_clause_final");
        }
    ;

if_clause
    : IF OPAREN expression CPAREN func_block {
            PTRACE("if_clause");
        }
    ;

else_clause
    : ELSE OPAREN expression CPAREN func_block {
            PTRACE("else_clause");
        }
    ;

else_clause_list
    : else_clause {
            PTRACE("else_clause_list:else_clause");
        }
    | else_clause_list else_clause {
            PTRACE("else_clause_list:else_clause_list");
        }
    ;

else_clause_final
    : ELSE OPAREN CPAREN func_block {
            PTRACE("else_clause_final:(){}");
        }
    | ELSE func_block {
            PTRACE("else_clause_final:{}");
            }
    ;

while_clause
    : WHILE OPAREN expression CPAREN func_block {
            PTRACE("while_clause");
        }
    ;

do_clause
    : DO func_block WHILE OPAREN expression CPAREN {
            PTRACE("do_clause");
        }
    ;

for_clause
    : FOR OPAREN SYMBOL IN compound_reference CPAREN func_block {
            PTRACE("for_clause:in");
        }
    | FOR OPAREN SYMBOL IN expression TO expression CPAREN func_block {
            PTRACE("for_clause:in:to");
        }
    ;

switch_case_clause
    : switch_clause case_clause_list case_clause_final {
            PTRACE("switch_case_clause");
        }
    ;

switch_clause
    : SWITCH OPAREN expression CPAREN OBLOCK {
            PTRACE("switch_clause");
        }
    ;

case_clause
    : CASE OPAREN const_expression CPAREN func_block {
            PTRACE("case_clause");
        }
    ;

case_clause_list
    : case_clause {
            PTRACE("case_clause_list:case_clause");
        }
    | case_clause_list case_clause {
            PTRACE("case_clause_list:case_clause_list");
        }
    ;

case_clause_final
    : CBLOCK {
            PTRACE("case_clause_final");
        }
    | DEFAULT func_block CBLOCK {
            PTRACE("case_clause_final:default");
        }
    ;

try_except_clause
    : try_clause except_clause_list except_clause_final {
            PTRACE("try_except_clause");
        }
    ;

try_clause
    : TRY func_block {
            PTRACE("try_clause");
        }
    ;

except_clause
    : EXCEPT OPAREN compound_name CPAREN func_block {
            PTRACE("except_clause");
        }
    ;

except_clause_list
    : except_clause {
            PTRACE("except_clause_list:except_clause");
        }
    | except_clause_list except_clause {
            PTRACE("except_clause_list:except_clause_list");
        }
    ;

except_clause_final
    : EXCEPT OPAREN CPAREN func_block {
            PTRACE("except_clause_final(){}");
        }
    | EXCEPT func_block {
            PTRACE("except_clause_final{}");
        }
    ;

%%

/*
 *  This converts terminal symbol definitions to strings for error handling.
 */
static const char *
symbol_name (yysymbol_kind_t yysymbol)
{
    return ((yysymbol == YYSYMBOL_YYEMPTY)? "empty":
        (yysymbol == YYSYMBOL_YYEOF)? "end of file" :
        (yysymbol == YYSYMBOL_YYerror)? "error" :
        (yysymbol == YYSYMBOL_YYUNDEF)? "invalid token" :
        (yysymbol == YYSYMBOL_NAMESPACE)? "NAMESPACE keyword" :
        (yysymbol == YYSYMBOL_CLASS)? "CLASS keyword" :
        (yysymbol == YYSYMBOL_PUBLIC)? "PUBLIC keyword" :
        (yysymbol == YYSYMBOL_PRIVATE)? "PRIVATE keyword" :
        (yysymbol == YYSYMBOL_PROTECTED)? "PROTECTED keyword" :
        (yysymbol == YYSYMBOL_CREATE)? "CREATE keyword" :
        (yysymbol == YYSYMBOL_DESTROY)? "DESTROY keyword" :
        (yysymbol == YYSYMBOL_INTEGER)? "INTEGER keyword" :
        (yysymbol == YYSYMBOL_UNSIGNED)? "UNSIGNED keyword" :
        (yysymbol == YYSYMBOL_FLOAT)? "FLOAT keyword" :
        (yysymbol == YYSYMBOL_STRING)? "STRING keyword" :
        (yysymbol == YYSYMBOL_DICT)? "DICT keyword" :
        (yysymbol == YYSYMBOL_LIST)? "LIST keyword" :
        (yysymbol == YYSYMBOL_BOOLEAN)? "BOOLEAN keyword" :
        (yysymbol == YYSYMBOL_NOTHING)? "NOTHING keyword" :
        (yysymbol == YYSYMBOL_IF)? "IF keyword" :
        (yysymbol == YYSYMBOL_ELSE)? "ELSE keyword" :
        (yysymbol == YYSYMBOL_WHILE)? "WHILE keyword" :
        (yysymbol == YYSYMBOL_DO)? "DO keyword" :
        (yysymbol == YYSYMBOL_FOR)? "FOR keyword" :
        (yysymbol == YYSYMBOL_IN)? "IN keyword" :
        (yysymbol == YYSYMBOL_TO)? "TO keyword" :
        (yysymbol == YYSYMBOL_AS)? "AS keyword" :
        (yysymbol == YYSYMBOL_TRY)? "TRY keyword" :
        (yysymbol == YYSYMBOL_EXCEPT)? "EXCEPT keyword" :
        (yysymbol == YYSYMBOL_RAISE)? "RAISE keyword" :
        (yysymbol == YYSYMBOL_RETURN)? "RETURN keyword" :
        (yysymbol == YYSYMBOL_EXIT)? "EXIT keyword" :
        (yysymbol == YYSYMBOL_SWITCH)? "SWITCH keyword" :
        (yysymbol == YYSYMBOL_CASE)? "CASE keyword" :
        (yysymbol == YYSYMBOL_DEFAULT)? "DEFAULT keyword" :
        (yysymbol == YYSYMBOL_YIELD)? "YIELD keyword" :
        (yysymbol == YYSYMBOL_TRACE)? "TRACE keyword" :
        (yysymbol == YYSYMBOL_PRINT)? "PRINT keyword" :
        (yysymbol == YYSYMBOL_IMPORT)? "IMPORT keyword" :
        (yysymbol == YYSYMBOL_CONTINUE)? "CONTINUE keyword" :
        (yysymbol == YYSYMBOL_ENTRY)? "ENTRY keyword" :
        (yysymbol == YYSYMBOL_BREAK)? "BREAK keyword" :
        (yysymbol == YYSYMBOL_TRUE)? "TRUE keyword" :
        (yysymbol == YYSYMBOL_FALSE)? "FALSE keyword" :
        (yysymbol == YYSYMBOL_NOT)? "NOT operator" :
        (yysymbol == YYSYMBOL_OR)? "OR operator" :
        (yysymbol == YYSYMBOL_AND)? "AND operator" :
        (yysymbol == YYSYMBOL_EQU)? "== operator" :
        (yysymbol == YYSYMBOL_POW)? "^ operator" :
        (yysymbol == YYSYMBOL_NEQU)? "!= operator" :
        (yysymbol == YYSYMBOL_LORE)? "<= operator" :
        (yysymbol == YYSYMBOL_GORE)? ">= operator" :
        (yysymbol == YYSYMBOL_ADD_ASSIGN)? "+= operator" :
        (yysymbol == YYSYMBOL_SUB_ASSIGN)? "-= operator" :
        (yysymbol == YYSYMBOL_MUL_ASSIGN)? "*= operator" :
        (yysymbol == YYSYMBOL_DIV_ASSIGN)? "/= operator" :
        (yysymbol == YYSYMBOL_MOD_ASSIGN)? "%= operator" :
        (yysymbol == YYSYMBOL_ASSIGN)? "= operator" :
        (yysymbol == YYSYMBOL_ADD)? "+ operator" :
        (yysymbol == YYSYMBOL_SUB)? "- operator" :
        (yysymbol == YYSYMBOL_MUL)? "* operator" :
        (yysymbol == YYSYMBOL_DIV)? "/ operator" :
        (yysymbol == YYSYMBOL_MOD)? "% operator" :
        (yysymbol == YYSYMBOL_DOT)? ". operator" :
        (yysymbol == YYSYMBOL_COMMA)? "," :
        (yysymbol == YYSYMBOL_COLON)? ":" :
        (yysymbol == YYSYMBOL_OPOINT)? "<" :
        (yysymbol == YYSYMBOL_CPOINT)? ">" :
        (yysymbol == YYSYMBOL_CPAREN)? ")" :
        (yysymbol == YYSYMBOL_OPAREN)? "(" :
        (yysymbol == YYSYMBOL_OBLOCK)? "{" :
        (yysymbol == YYSYMBOL_CBLOCK)? "}" :
        (yysymbol == YYSYMBOL_OBRACE)? "[" :
        (yysymbol == YYSYMBOL_CBRACE)? "]" :
        (yysymbol == YYSYMBOL_SYMBOL)? "Symbol" :
        //(yysymbol == YYSYMBOL_COMPOUND)? "Compound Symbol" :
        (yysymbol == YYSYMBOL_STRG_CONST)? "String literal" :
        (yysymbol == YYSYMBOL_FLOAT_CONST)? "Float literal" :
        (yysymbol == YYSYMBOL_INT_CONST)? "Integer literal" :
        (yysymbol == YYSYMBOL_INLINE)? "Inline block" :
        (yysymbol == YYSYMBOL_UNSIGNED_CONST)? "Unsigned literal" : "UNKNOWN");
}

/*
 * This looks hacky but it's official: I found it here:
 * https://www.gnu.org/software/bison/manual/html_node/Syntax-Error-Reporting-Function.html
 */
static int
yyreport_syntax_error (const yypcontext_t *ctx)
{
  int res = 0;
  //YYLOCATION_PRINT (stderr, yypcontext_location (ctx));
  fprintf(stderr, "%s:%d:%d: syntax error",
                get_file_name(), get_line_no(), get_col_no() );
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
        fprintf (stderr, "%s %s",
                 i == 0 ? ": expected" : " or", symbol_name (expected[i]));
  }
  // Report the unexpected token.
  {
    yysymbol_kind_t lookahead = yypcontext_token (ctx);
    if (lookahead != YYSYMBOL_YYEMPTY)
      fprintf (stderr, " before %s", symbol_name (lookahead));
  }
  fprintf (stderr, "\n");
  return res;
}

void yyerror(const char* s) {

    fprintf(stderr, "%s:%d:%d %s\n",
            get_file_name(), get_line_no(), get_col_no(), s);
    //error("%s", s);
}

const char* tokenToStr(int tok) {
    // This is the official way...
    return yysymbol_name(YYTRANSLATE(tok));
}
