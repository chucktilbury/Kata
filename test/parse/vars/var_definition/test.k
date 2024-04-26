;------------------------------------------------------------------------------
; this tests that floats are properly recognized.
;
;   var_definition
;       = ( 'var' / 'variable' ) var_decl ( '=' assignment_item )?
;
; Note that assignment_item is tested elsewhere.
;------------------------------------------------------------------------------

var float some_name = "0.0"
var float some_name = 1
var float some_name = 1.1
var float some_name = 123123123123123123.0
var float some_name = 0.123123123123123123
; the '-' is a unary minus in the expression
var float some_name = -1.0e12
var float some_name = 10e-2
var float some_name = 10.1e-12
var float some_name = 10e2
var float some_name = 10.1e12
