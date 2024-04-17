;-------------------------------------------------------------------------------
; Test using a formatted string with invalid args in a string expression
;
; Missing a comma between args. Produce a syntax error in the context of a
; formatted string.
;-------------------------------------------------------------------------------

var string some_name = "this is a string"(1 2, 3)
