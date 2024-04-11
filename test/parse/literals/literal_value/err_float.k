;------------------------------------------------------------------------------
; Test if the scanner detects an error in a float
;------------------------------------------------------------------------------

var float some_name 123.123.12
var float some_name 123..12
var float some_name 123.
var float some_name .123.
