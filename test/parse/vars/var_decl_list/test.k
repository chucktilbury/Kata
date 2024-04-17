;------------------------------------------------------------------------------
; This tests the var_decl list. This can only be done in the context of a 
; function definition.
;
;   var_decl_list
;       = '(' ( var_decl ( ',' var_decl )* )? ')'
;
;------------------------------------------------------------------------------

func some_name()() {}
func some_name(float some_name)(string some_name)  {} 
func some_name(bool some_name)(list some_name) {}
func some_name(signed some_name)(dict some_name) {}
func some_name(float name1, string name2, bool name3)(const string name4) {}
func some_name(float name1)(string name2, signed name3, unsigned name4) {} 
func some_name(compound.name name1)(this.is.another.name name2) {} 



