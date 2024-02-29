# Simple
The Simple programming language.

This is a very simple object-oriented programming language compiler which is written in C. It's a fun hobby project. See the grammar below. It's pretty complete and I am actively working on a parser and AST for it. (January, 2024)

The language is a synthesis or Python and C. It has the object model of Python but shares a lot of syntax with both C and C++.

General features in no particular order.
* Curly braces to delimit blocks. Semicolons are used to introduce single-line comments. Whitespace is completely ignored except to separate some tokens.
* Simplified type system, but is strongly typed with casting.
* Durable typing. Once a type is specified, it cannot be changed. Casting is supported and a different var can be created with the different type. The exception to this is the ``nothing`` type. This allows the type to be assigned at runtime, and once it is assigned, then the type of the variable becomes durable and is enforced at runtime.
* Function overloads by parameter type. Functions have a syntax of ``name(input parameters)(output parameters)``. Input parameters are passed to the function by value and output parameters are passed as references. Outputs not assigned in the function is a warning. Outputs accessed by value in a function is a syntax error, and duplicate names are a syntax error. This means that a function does not "return" anything. A syntax such as ``name.name.func()().name`` makes no sense. Use exceptions to communicate errors and such.
* Functions can be overridden by the signature, including the return values.
* Functions can be assigned to variables using a syntax such as ``nothing varname = func_name()()``.
* The ``nothing`` type takes the place of the ``void`` type in C. This is used to allow an data object to have a type assigned at run time.
* Classes with single inheritance only. Class definitions can contain no assignments or function definitions. The parent attributes and methods are accessed by name with the '.' operator. The scope operators set the scope of subsequent lines of code. The default scope is ``private``.
* Membership is given by the '.' operator and spaces are allowed before or after it.
* Virtual functions. If a virtual function has a definition in its class and a child class inherits it, then the child can still access the parent method using the membership '.' operator. Overriding a virtual function is optional. Defining a virtual function is optional. Accessing a virtual function that has not been defined is a runtime error.
* Supports dictionaries and lists natively.
* String is a native data type and all strings can be formatted without an explicit method call.
* The compiler and the runtime library both use managed memory implemented by the Boehm-Demers-Weiser Garbage Collector found here: https://www.hboehm.info/gc/
* Exceptions using the non-local goto feature of ANSI C. A string error message is passed back to the exception handler.
* Support for external libraries using an ``inline`` keyword that causes literal code to be copied directly to the output without any checking. This is implemented as a scanner feature.
* Constant values cannot be changed once they are assigned, but they are assigned at runtime.

Features that are specifically not supported.

* Pointers.
* Multiple inheritance.
* Monkey patching.

This multi-pass compiler accepts Simple source code and outputs C source code. The output should be compilable by any modern ANSI C compiler. The output is not really intended to be human readable, but the #line directives are used to aid debugging. The generated code favors efficiency over readability. The goal is that there should be no way for the compiler to output incorrect code. All semantic errors are caught before emitting any output, except for the inline functionality. No checking or parsing is done on that whatsoever.

# This repo
(assuming you know how to use github)
This repo has not nor ever will be built by me under any version of Windows. It should be usable on other operating systems. I don't have a MAC, so I can't test anything. In general it should build under Linux without problems. There are a couple of files to make it easier to use. After you clone it, take a look at the .project_env file to see if it matches your setup. If it does, then you can just run ``./setup`` to check out the sub projects and create a sub-shell with some paths set up. To make the repo perfectly clean, run the ``./clean`` script or delete the docs/out, build, and bin directories.

## required packages
This requires an ANSI C compiler, cmake, graphiz, and doxygen. No other libraries or tools should be required.

## Feedback is most welcome
I would also welcome some help. Compilers are such a slog....

# Grammar
This is the complete and updated grammar for Simple:
```

#####################
#
# This is a simplified grammar that is based upon BNF. It is intended to
# illustrate the physical syntax of the Simple programming language. It
# is not expected that this grammar will change very much. This is not
# intended to be a "regular" grammar. There are several conflicts that include
# a big one between a compound_name and a compound_reference. This is most
# easily resolved using a hand coded parser, so no effort has been put into
# making this grammar conform to any parser generator input syntax.
#
# Note that comments are introduced with a ';' character and extend to the
# first newline encountered.
#
# The items that are in parentheses are followed by a '?', '+', or a '*'.
# These are used to simplify the text of the grammar and represent what would
# be normally expected in a regular expression. I.e. '?' represents "zero or
# one", '+' represents "one or more", and '*' represents "zero or more". The
# slash character '/' should be read as the word 'or'.
#
# Keywords a shown as string literals and are NOT case-sensitive. Constructed
# (such as a SYMBOL = [a-zA-Z_][A-Za-z0-9_]*) terminal symbols are in
# all-caps.
#
#####################

#####################
#
# A module is the top-level production. It is a list of module items.
#
module
    = ( module_item )+

#####################
#
# Module items are everything that can be defined in a namespace plus the
# import statements and the start function. There must be exactly one start
# function in a program. A program cannot build without it. The import
# statement brings in the names from other modules to facilitate linking with
# them but does not bring in the source code. Imported objects are accesses
# according to their namespace using the '.' operator.
#
module_item
    = namespace_item
    / import_statement
    / start_function

#####################
#
# There must be exactly one start function in a program. This is where the
# program execution begins.
#
start_function
    = 'function' 'start' function_body

#####################
#
# Import statement must always be at the module level. An import is treated as
# a separate module and only the names are read in so that they can be
# referenced. The imported module is then linked by the compiler to make the
# program.
#
import_statement
    = 'import' formatted_strg AS SYMBOL

#####################
#
# A namespace is simply a way to manage names and prevent conflicts. Functions
# and data can be defined in a namespace that are not a part of a class. Names
# that are defined in a namespace are considered to be in the local scope.
#
namespace_item
    = scope_operator
    / namespace_definition
    / class_definition
    / func_definition
    / var_definition

#####################
#
# The scope operator sets the current scope that the compiler is using. The
# default scope is PRIVATE. When the scope operator is used at the module
# level, only the import statements are effected. The current scope must be
# PUBLIC to allow symbols to be imported. The PROTECTED scope is not valid at
# the module level and causes a warning and is ignored.
#
# At the namespace level, the PROTECTED scope is invalid and produces a
# warning. In the PRIVATE scope, objects defined will not be accessible outside
# of the namespace.
#
# At the class level, all 3 operators are valid. As one would expect, the
# PROTECTED scope allows names to be seen locally, and when the class has been
# inherited. Note that constructors and destructors are always protected.
#
scope_operator
    = 'private'
    / 'public'
    / 'protected'

#####################
#
# A literal type name is a keyword that represents the name of a builtin type.
#
literal_type_name
    = 'float'
    / 'unsigned'
    / 'signed'
    / 'string'
    / 'boolean'
    / 'nothing'
    / 'list'
    / 'dict'
    / 'function'

#####################
#
# A type name is a literal type or a user-defined type that can be named by a
# compound name.
#
type_name
    = literal_type_name
    / compound_name

#####################
#
# Most strings in Simple are presented as formatted. There is no need to call
# a function to format a string. Strings that are enclosed in double-quotes
# have escape characters interpreted, included hex characters. Single quote
# strings are absolute literals. Strings can span multiple lines. Double quote
# strings require the newlines to be inserted manually and line breaks in the
# definition are ignored. In single quote strings, the line breaks are
# inserted. String format expressions are positional and marked by a '{}'
# token. The string format is optional.
#
# These syntaxes are valid:
# "string {} something"() ; This formats the '{}' into the string
# "string {} something"(1+2*3) ; "string 7 something"
# "string {} some" (1+2, "blart") ; produce an extra parameter warning
# 'string {} some'(1+2) ; produce format ignored warning
#
formatted_strg
    = LITERAL_DSTRG (expression_list)?

#####################
#
# An SSTRG is an absolute literal, but it can be assigned to a DSTRG to have
# it formatted at a later time.
#
string_literal
    = LITERAL_SSTRG
    / formatted_strg

#####################
#
# Items that can be in a string expression.
#
string_expr_item
    = string_literal
    / compound_reference
    / literal_value

#####################
#
# This is a way to concatenate strings with various types of objects.
#
string_expr
    = string_expr_item ( '+' string_expr_item )*

#####################
#
# All numbers are doubles
# A bool is "true" or "false" keywords.
#
literal_value
    = LITERAL_FLOAT
    / LITERAL_UNSIGNED
    / LITERAL_SIGNED
    / LITERAL_BOOL

#####################
#
# A variable declaration is simply a type and a name. The type can be named by
# a compound_name.
#
var_decl
    = ( 'const' )? type_name SYMBOL

#####################
#
# This non-terminal is used for things like function parameter definitions.
#
var_decl_list
    = var_decl ( ',' var_decl )*

#####################
#
# An assignment item is an item that can be used to initialize a variable.
#
assignment_item
    = expression
    / list_init
    / dict_init
    / string_expr

#####################
#
# A variable definition has an optional assignment.
#
var_definition
    = var_decl ( '=' assignment_item )?

#####################
#
# A list init is used to initialize a list at the time that the program starts
# up. A list initialization looks like this:
#
# [1, 2, 3]
# [a, [1, 2, 3], ["asd":1, "qwe":2, "zxc":3]] ; note that 'a' has to be a
# compile-time initialized constant.
#
list_init
    = '[' assignment_item ( ',' assignment_item )* ']'

#####################
#
# Dictionary initializer is a literal string, followed by a ':', followed by
# a assignment item.
#
dict_init_element
    = ( LITERAL_DSTRG / LITERAL_SSTRG ) ':' assignment_item

#####################
#
# A dictionary initializer
#
# Examples:
# ["qwe":[1,2,3], "asd":some.compound.name, "zxc":["qwe":different.key]]
#
dict_init
    = '[' dict_init_element ( ',' dict_init_element )* ']'

#####################
#
# Array parameter for an array reference. This is right-recursive rule that
# allows syntax like asd[2][3].
#
array_param
    = '[' ( expression / string_expression ) ']' ( array_param )*

#####################
#
# An array reference is part of a compound_reference.
#
array_reference
    = SYMBOL array_param

#####################
#
# Function references have a list of return parameters are well as the calling
# parameters. First the calling parameter list, then the return parameters.
# Calling parameters are passed by value and return parameters are passed by
# reference. If call or return parameters are not referenced in a function then
# a syntax warning is issued. These warnings can be suppressed by casting the
# parameter to the NOTHING data type. If they are cast to NOTHING, then they
# cannot be referenced in the function. Functions cannot participate in an
# expression because they do not return a value directly.
#
# Examples:
# name()() ; function 'name' takes no parameters and returns no values.
# some.name(12, "asd")() ; function uses parameters, but returns nothing
# directly.
# some.name()(12, asd.qwe) ; syntax error because 12 does not name a variable
#
function_reference
    = compound_name
            ( expression_list )+
            '(' ( compound_name ( ',' compound_name )* )+ ')'

#####################
#
# A compound name can name a type or it can name a variable. The compiler is
# to check for correctness at compile time.
#
compound_name
    = SYMBOL ( '.' SYMBOL )*

#####################
#
# A compound reference must name a variable that is suitable to have a value
# assigned to it. Note that unlike other languages, Simple does not allow
# function references in a compound reference.
#
compound_ref_item
    = SYMBOL
    / array_reference

#####################
#
# Compound reference may contain a reference to an array, and may not name a
# data type.
#
compound_reference
    = compound_ref_item ( '.' compound_ref_item )*

#####################
#
# Head of an expression parse. The OR keyword is the lowest precedence
# operator. This operator only understands boolean values. When this is
# encountered in an expression, all operands are converted to bool. Expressions
# are actually implemented using the shuntinting yard algorithm. The segmented
# representation here is to show the precedence of the operators.
#
expression
    = expr_and 'or' expr_or

#####################
#
# And operator is the next highest precedence. Boolean only.
#
expr_and
    = expr_equ 'and' expr_and

#####################
#
# Test for equality. These operators compare equality on numeric values as
# well as string values. If a class has defined an equality operator then
# this will use it to test equality on same-type objects.
#
expr_equ
    = expr_mag '==' expr_equ
    / expr_mag '!=' expr_equ

#####################
#
# Magnitude operators work on numeric values and on same-type objects that
# have defined methods for these operators.
#
expr_mag
    = expr_term '<' expr_mag
    / expr_term '>' expr_mag
    / expr_term '<=' expr_mag
    / expr_term '>=' expr_mag

#####################
#
# Add and subtract operators work on numbers and strings. Also work on objects
# that have defined methods.
#
expr_term
    = expr_fact '+' expr_term
    / expr_fact '-' expr_term

#####################
#
# Factor operators work on numbers and objects that have defined methods.
#
expr_fact
    = expr_pow '*' expr_fact
    / expr_pow '/' expr_fact
    / expr_pow '%' expr_fact

#####################
#
# Power operator is high precedence and operates on 2 numbers only. For
# example, 2^10 = 1024
#
expr_pow
    = expr_unary '^' expr_pow

#####################
#
# Unary operators. The '!' operates on boolean values only. The '-' operator
# operates on numbers only.
#
expr_unary
    = '!' expr_primary
    / '-' expr_primary

#####################
#
# A primary value to use in an expression. A compound reference must name a
# data element. The '(' expression ')' changes the precedence of an operation
# and the ( type_name )? specifies a casting operation. Casting operations can
# be used in a normal expression.
#
expr_primary
    = literal_value
    / compound_reference
    / ( type_name )? '(' expression ')'

#####################
#
# The expression list is normally used for things like parameter lists. The
# parens are required and the expression list is optional.
#
expression_list
    = '(' (expression ( ',' expression )*)? ')'

#####################
#
# A namespace is a way to separate similarly named spaces into separate
# spaces. The items in the namespace are syntactically optional, but the
# '{}' is not optional.
#
namespace_definition
    = 'namespace' SYMBOL '{' ( namespace_item )+ '}'

#####################
#
# A class definition contains a list of class items. It accepts a single
# optional parameter and the '()' around the parameter is optional. Also
# the items in the class are optional. In the Simple language, a class can
# only be declared once, and declaring an empty class prevents it from
# being declared elsewhere.
#
class_definition
    = 'class' SYMBOL ( '(' ( type_name )? ')' )?
            '{' ( class_item )+ '}'

#####################
#
# Class items are simple. A constant variable declaration means that its value
# is constant for all objects that are created from it and it may not be
# changed or cast to another type. The virtual functions may have a definition,
# but if the function is defined by a class that inherits this one, it will be
# redefined and it can still be called using the name of the parent class and
# the '.' operator. Normally, if a function is declared in a class declaration,
# then not having a definition for it is a compile time error. A class
# declaration cannot contain a function definition. The const variable is a
# constant only for that object.
#
class_item
    = scope_operator
    / var_decl
    / func_decl

#####################
#
# There are 3 types of functions that can be declared. Functions are decorated
# according to the parameters that they are declared with, so functions can be
# overloaded. The constructor cannot return anything and the destructor cannot
# have any parameters, but it must be declared if a definition is provided. Of
# course the default constructors and destructors are provided.
#
func_decl
    = ( 'virtual' )? 'function' SYMBOL 
            '(' ( var_decl_list )* ')' 
            '(' ( var_decl_list )* ')'
    / ( 'virtual' )? 'function' 'create' 
            '(' ( var_decl_list )* ')'
    / ( 'virtual' )? 'function' 'destroy'

#####################
#
# The function definition is just like the declaration except that it must be
# tied to the class that it was declared in, if it exists. A normal function
# does not need to tie to a class, but a compound name needs to name a type
# such that this function can be defined for it. However, a compound name can
# be a single symbol, meaning that it does not tie to a class.
#
func_definition
    = 'function' compound_name
            '(' ( var_decl_list )* ')' '(' ( var_decl_list )* ')' function_body
    / 'function' compound_name '.' 'create' '(' ( var_decl_list )* ')' function_body
    / 'function' compound_name '.' 'destroy' function_body

#####################
#
# A function body contains an optional list of elements that can appear in a
# function.
#
function_body
    = '{' ( function_body_element )* '}'

#####################
#
# Assignment is not part of the expressions as it is in most other languages.
# The simple assignment can do anything that names a type. Type compatibility
# is checked at runtime. The other assignment operators only work with numbers
# except for the '+=' operator, which can concatenate a string.
#
assignment
    = compound_reference '=' assignment_item
    / compound_reference '+=' expression
    / compound_reference '-=' expression
    / compound_reference '*=' expression
    / compound_reference '/=' expression
    / compound_reference '%=' expression

#####################
#
# See the individual items for more information about them.
#
function_body_element
    = var_definition
    / function_reference
    / assignment
    / while_clause
    / do_clause
    / for_clause
    / if_clause
    / try_clause
    / switch_clause
    / break_statement
    / continue_statement
    / trace_statement
    / inline_statement
    / yield_statement
    / type_statement
    / exit_statement
    / print_statement
    / return_statement
    / raise_statement
    / function_body

#####################
#
#
break_statement
    = 'break'

#####################
#
#
continue_statement
    = 'continue'

#####################
#
#
trace_statement
    = 'trace'

#####################
#
#
inline_statement
    = 'inline' '{' RAW_TEXT '}'

#####################
#
#
yield_statement
    = 'yield' '(' compound_reference ')'

#####################
#
#
type_statement
    = 'type' '(' compound_reference ')'

#####################
#
#
exit_statement
    = 'exit' '(' ( expression )? ')

#####################
#
#
print_statement
    = 'print' ( expression_list )?

#####################
#
#
return_statement
    = 'return' ( '(' ( expression )? ')' )?

#####################
#
#
raise_statement
    = 'raise' '(' SYMBOL ',' formatted_strg ')'

#####################
#
# This simplifies a while definition. The parens and the expression are
# optional. If they are not present, or if the parens are present but not
# the expression, then it is taken as true, same as while(1){}. String
# expressions are not allowed. Expressions that evaluate to a float are
# always true because floats theoretically can never be exactly zero. This
# creates a runtime warning. The expression must evaluate to a boolean, a
# signed or unsigned in order to be symantically valid.
#
while_definition
    = 'while' ( '(' ( expression )? ')' )?

#####################
#
# Implement the while() {} clause. Note that an empty or missing expression
# is an endless loop.
#
while_clause
    = while_definition function_body

#####################
#
# The 'do' clause is the same as it is in the C language. As with the 'while'
# clause, a missing or blank expression is an endless loop.
#
do_clause
    = 'do' function_body while_definition

#####################
#
# For clause is not like C or Python. If the expressions are missing then it's
# the same as 'while {}'. The expression is evaluated after to loop is run and
# the SYMBOL is updated. The expression is then cast to a bool. If it evaluates
# to true, then the loop is entered again otherwise the function body is
# skipped and execution continues after the end of it. Strings are not allowed
# in the expression. if the expression evaluates to a float, then a runtime
# warning is generated because the only way for the expression to evaluate to
# false as a float is if it is exactly 0.0, which is theortically impossible.
# To be symantically correct, expression has to evaluate to a signed, unsigned,
# or a bool. Note that casting is allowed.
#
for_clause
    = 'for' ( '(' (type_name)? SYMBOL 'in' expression ')' )?
            function_body

#####################
#
# The first 'if' clause requires an expression. The expression is cast to
# boolean and if it's 'true' then the function body is executed and the 'else'
# clauses, if any, are ignored. If the first 'if' is false, then the second
# one is tried and so on until a 'true' can be evaluated. All of these 'if'
# statements require an expression except for the last one, where it's
# optional, since blank or missing expressions are always true. Only the last
# else clause can have a blank (and therefore true) expression. This policy is
# enforced by the parser, rather than with a compile time policy. Note that
# the else_clause is optional.
#
if_clause
    = 'if' '(' expression ')' function_body else_clause

#####################
#
# Else clause with a required expression.
#
else_clause_mid
    = 'else' '(' expression ')' function_body

#####################
#
# Else clause with a blank or missing expression must be last.
#
else_clause_final
    = 'else' ( '(' ')' )? function_body

#####################
#
# Actual else clause list definition. Zero or more mid clauses and zero or one
# final clauses. Note that a stand-alone if() is syntattically correct.
#
else_clause
    = ( else_clause_mid )* ( else_clause_final )?

#####################
#
# Try clause has a function body and one or more except clauses. It could be
# a single else with no SYMBOL that catches any exception. Since the internal
# exception system uses the C non-local GOTO functionality, the SYMBOL that is
# required names an internal ENUM that is used to longjmp() to the correct
# location. One or more except clauses can be present.
#
try_clause
    = 'try' function_body except_clause

#####################
#
# Except clause with a required symbol. The first symbol names which exception
# is going to be caught. The second one is taken to be a string var that is an
# error message that was given when the exception was raised. To capture any
# exception, the exception type should be 'any' and the error message is
# propagated as expected.
#
except_clause_mid
    = 'except' '(' SYMBOL ',' SYMBOL ')' function_body

#####################
#
# The final clause matches any exception, but it must be the last one that
# appears in the list of clauses.
#
except_clause_final
    = 'except' '(' 'any' ',' SYMBOL ')' function_body

#####################
#
# The except clause must have at least one element, but that element could be a
# mid or a final. There may be any number of mid clauses. A final clause must
# be the last one. If there are no middle except clauses then the final one is
# required. In other words, at least one except clause is required and if there
# is an "any" except clause, then it needs to be the last one to be
# syntattically correct.
#
except_clause
    = ( except_clause_mid )+ ( except_clause_final )?
    / except_clause_final

#####################
#
# The switch/case construct is similar to the one in C except that it accepts
# strings as well as numbers.
#
switch_clause
    = 'switch' '(' compound_reference ')' case_body

#####################
#
# A case clause must have a literal value or a data reference that was
# declared as CONST. First the match is made against the type that appears in
# the switch clause. If the type does not match, then the case cannot match.
# There is an exception in that a signed and an unsigned are considered to be
# the same type for a case clause.
#
case_clause
    = 'case' '(' ( literal_value / LITERAL_DSTR / LITERAL_SSTR ) ')' function_body

#####################
#
# The default clause is the last thing if it's present. If none of the cases
# match, then the default clause is matched and executed. If the default clause
# is not present and none of the cases match, then the switch is not executed.
#
default_clause
    = 'default' function_body

#####################
#
# The actual case body definition is similar to C. One or more 'case' clauses
# followed by exactly one optional 'default' clause.
#
case_body
    = '{' ( case_clause_list )+ ( default_clause )? '}'

```
