# TODO list

## Current
* Symbol table. Track the actual chain of symbols in the symbol table and place the full symbol in the table with a pointer to the AST entry. Each segment in the table is its own symbol. The symbol table is global and every symbol is unique. 

### Actual bugs
* When the scanner receives a float with the format of 123.123.123, then it correctly scans the float and then returns the '.' as the next token. However, the parser does not produce a correct error in that case.
* Scanner incorrectly sees a number such as "123..123" as a float, when it should flag an error. and stop scanning at the first '.'
* Scanner incorrectly sees a number such as "123." as a float, when it should flag an error. and stop scanning at the first '.'


### Improvements
* Do not allow spaces around a '.' in a compound name or reference.

#### Error messages
* var declaration missing from function body
* Add a test that uses '()' in a formatted string expression list.
* Improve error message where a string_expr_list follows a LITERAL_SSTRG.
* Fix error message where a "start" is defined within a function.

#### Imports
* Need a way to add path entries at build time so that imports can be located.
  * create for command line
* The include file_find needs a way to add paths to it at runtime.

#### Error recovery needs 
* Consume tokens to prevent multiple nonsense errors.
  * This is done but needs more work
* Need a way to reset the parser to start over after errors.

#### Modules
* sys module contains stuff like trace and print
  * should exit() be a part of that?
  * Implications for normal function format.
* stream module contains access to files and stdio.
* math module implements the functions

## Done
* Improve error message where final else is not the last one.
* Change if/else to be if/else/final
* Change switch/case/default to be switch/case/final and delete the 'default' keyword
* Simple assignment as in "name = name" appears to not be a function_body_item.
* **important** Improve error messages when a error token is received by the parser from the scanner. 
  * Example, when an invalid unsigned is scanned, such as 0xx123.
  * go through the scanner and map every instance of an TOK_ERROR return into the parser.
    * See func_body.inline for example
* String expressions such as ``strg == strg``, ``strg != strg``, ``strg = strg + strg``, and ``strg += strg``
* String expressions and other expressions cannot mix??
  * If this is a semantic question then strgs need to be added to the ``primary_expression`` productions.


