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


var signed some_name = 0
var signed some_name = 1
var signed some_name = 10
var signed some_name = 123123123123
; The minus is an unary minus in the expression and not a part of the number.
var signed some_name = -1
var signed some_name = -12398123