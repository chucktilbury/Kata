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


var unsigned some_name = 0x0
var unsigned some_name = 0X0
var unsigned some_name = 0x01
var unsigned some_name = 0x0000
var unsigned some_name = 0xaBc0
var unsigned some_name = 0x0123
var unsigned some_name = 0xabcdef12
