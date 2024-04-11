;------------------------------------------------------------------------------
; This is literal_type_name test. There are no syntax error conditions
; to test in this production.
;
;   literal_type_name
;       = 'float'
;       / 'unsigned'
;       / 'signed'
;       / 'string'
;       / 'boolean'
;       / 'nothing'
;       / 'list'
;       / 'dict'
;       / 'function'
;   
;   This really just makes sure that the tokens are seen.
;
;------------------------------------------------------------------------------

var float some_name
var unsigned some_name
var string some_name
var signed some_name
var boolean some_name
var nothing some_name
var list some_name
var dict some_name
var function some_name
