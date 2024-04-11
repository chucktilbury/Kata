* When the scanner receives a float with the format of 123.123.123, then it correctly scans the float and then returns the '.' as the next token. However, the parser does not produce a correct error in that case.

* Scanner incorrectly sees a number such as "123..123" as a float, when it should flag an error. and stop scanning at the first '.'

* Scanner incorrectly sees a number such as "123." as a float, when it should flag an error. and stop scanning at the first '.'

