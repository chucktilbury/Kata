;-------------------------------------------------------------------------------
; Basic if/elif/else clause
;-------------------------------------------------------------------------------

func name()() {

	if(required.expression) {
		some_func()()
	}
	elif(another.required.expression) {
		a.different.thing()() ; the word func is illegal in a reference
	}
	else { 
		; must not have an expression
		var const string name1
	}
}
