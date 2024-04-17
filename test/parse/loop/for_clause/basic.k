;-------------------------------------------------------------------------------
; Demonstrate all valid syntax
;
;   for_clause
;       = 'for' ( '(' ( (type_name)? SYMBOL 'in' expression )? ')' )?
;            function_body
;
;-------------------------------------------------------------------------------

func name ()() {

    for {}

    for() {}

    for ( name in something != 0 ) {}

    for ( signed name in something != 0 ) {}

}
