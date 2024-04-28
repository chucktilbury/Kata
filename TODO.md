# TODO list

* Add a test that uses '()' in a formatted string expression list.
* Error recovery needs to consume tokens to prevent multiple nonsense errors.
* Simple assignment as in "name = name" appears to not be a function_body_item.
* The include file_find needs a way to add paths to it at runtime.
* Improve error message where a string_expr_list follows a LITERAL_SSTRG.
* **important** Improve error messages when a error token is received by the parser from the scanner. 
  * Example, when an invalid unsigned is scanned, such as 0xx123.
* Fix error message where a "start" is defined within a function.
* When the scanner receives a float with the format of 123.123.123, then it correctly scans the float and then returns the '.' as the next token. However, the parser does not produce a correct error in that case.
* Scanner incorrectly sees a number such as "123..123" as a float, when it should flag an error. and stop scanning at the first '.'
* Scanner incorrectly sees a number such as "123." as a float, when it should flag an error. and stop scanning at the first '.'
* Need a way to add path entries at build time so that imports can be located.
* Need a way to reset the parser to start over after errors.




