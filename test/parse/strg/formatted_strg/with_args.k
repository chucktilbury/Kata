;-------------------------------------------------------------------------------
; Test using a formatted string with valid args in a string expression
;-------------------------------------------------------------------------------

var string some_name = 'this is a string' ; not a formatted string.
var string some_name = "this is a string"(1, 2, 3)

; TODO: Must add literal string to expression....
; var string some_name = "this is a string"(compound.name, "string", 'string')
