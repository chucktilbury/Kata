;------------------------------------------------------------------------------
; Test the var decl production.
;
;   var_decl
;       = ( 'const' )? type_name SYMBOL
;
; See type_name in literals.
;
;------------------------------------------------------------------------------


var float some_name
var const string some_name
var const some_compound_name some_name

