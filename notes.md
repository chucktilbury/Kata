
# Problems and thoughts...
---------------20240311---------------
* need to support the '+=' operator for arrays, dictionaries, and strings. This requires that an array reference could just be the name, rather than a subscript. Runtime issue?
* change function declarations to only have parameter type names. This is because function_declaration assignments have to use type names because of function overloading.

---------------20240227---------------
## AST passes. 
The actual compilation happens as a result of traversing the AST. I am planning several passes. For example, a pass that verifies that all of the symbols exist and that the types of them are reasonable. So, there will be a pass that places all of the symbols in a table, along with their fully qualified name and type, and all, according to the way that they are defined. The code that implements the pass is a table of functions that is run when a particular non-terminal is handled. In other words, a table is passed to the traverse_xxx() function and if that function has an entry in the table, then it is run with access to the actual current node.

---------------20240226---------------
* (done) Create boilerplate for all parser and ast functions and data structures.

---------------20240217---------------

* (done) Separate formatted strings from arithmetic expressions. Have a separate type of expressions that can have the '+' operator. That can be assigned or passed in a function reference.
  * Formatted strings use dquote strings and not squote strings. The scanner needs to be modified to accommodate the difference.
  * A squote string is an absolute literal and not subject to formatting.

* (done) Imports only read in public names for use in the module that does the import. Imports need to actually create a module instead of simply including the text.

---------------20240215---------------

## scanner modifications (done)
* Differentiate single and double quote strings.
* Add unsigned and signed int number type names.
* Differentiate numerical literals.
* Add "virtual" keyword.
* Tests.

---------------20231215---------------

## how the token queue works

* When the token queue is rewound, it should only step back as far as the current rule has been read. When alternatives are evaluated they need to start in the stream where the function was called. So it looks more like a stack than like a queue. This makes sense in view of how a LALR(n) operates. The difference here is that the stack is not evaluated directly, but in code so to speak.
  * When a terminal actually gets used and the AST node is returned, then mark it as used. Then at some point, check the token stream. If the one at the head is marked as used, then dispose of each one in order until one that has not been marked is found. Then make that the new head.

# obsolete
* The import keyword switches the input file and also creates a namespace with the name of the imported file. That means that the scanner needs to report when a file ends and the parser needs to parse for that to know when to end the namespace.
  * Change fileio.c so that instead of closing files automatically, it is closed manually. The parser then controls when the file is actually closed. New functions of push_input_file() and pop_input_file(). Instead of only returning a EOF token to represent the end of all input, return EOF as the end of a file and an EOI token to represent the end of the input. The end of input happens where the last file has been popped from the file stack.
  * The scanner needs to return the EOF and EOI as tokens so that the parser can see when to make a change to the current namespace.
  * <DONE> Added end of file and end of input tokens to the scanner and   verified that they are being properly returned.

## Steps to add a parser non-terminal target
1. Update the grammar in README.md
2. Add the AST type in AstType in ast.h
3. Add the data struture in the correct header in the ast directory.
4. Add the parse and traverse function prototypes.
5. Implement the parse and traverse functions.