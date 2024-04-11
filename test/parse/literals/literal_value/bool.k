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
; This is mostly a scanner test.
;
;------------------------------------------------------------------------------


var boolean some_name = True
var boolean some_name = TrUe
var boolean some_name = true
var boolean some_name = TRUE
var boolean some_name = On
var boolean some_name = ON
var boolean some_name = False
var boolean some_name = FaLsE
var boolean some_name = false
var boolean some_name = FALSE
var boolean some_name = Off
var boolean some_name = OFF
var boolean some_name = OfF

