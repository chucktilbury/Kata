;------------------------------------------------------------------------------
; The type name list is used only when declaring a function. This tests needs
; to use a lot of other productions.
;   
;   type_name_list
;       = '(' (type_name (',' type_name)* )? ')'
;   
;------------------------------------------------------------------------------

class some_name {

    func some_name()() ; could be blank
    func some_name(float)(string) 
    func some_name(bool)(list) 
    func some_name(signed)(dict) 
    func some_name(float, string, bool)(string) 
    func some_name(float)(string, signed, unsigned) 
    func some_name(compound.name)(this.is.another.name) 

}

