;------------------------------------------------------------------------------
; Literal values test. There is no valid syntax that allows these to appear 
; out of the context of an expression.
;
;   literal_value
;       = LITERAL_FLOAT
;       / LITERAL_UNSIGNED
;       / LITERAL_SIGNED
;       / LITERAL_BOOL
;
;------------------------------------------------------------------------------


var float some_name = 0.0
var float some_name = 1.0
var float some_name = 1.1
var float some_name = 123123123123123123.0
var float some_name = 0.123123123123123123
; the '-' is a unary minus in the expression
var float some_name = -1.0e12
var float some_name = 10e-2
var float some_name = 10.1e-12
var float some_name = 10e2
var float some_name = 10.1e12

