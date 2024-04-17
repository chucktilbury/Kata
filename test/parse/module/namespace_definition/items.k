;-------------------------------------------------------------------------------
; Test that correct items are accepted
;
;   namespace_item
;       = scope_operator
;       / namespace_definition
;       / class_definition
;       / function_definition
;       / create_definition
;       / destroy_definition
;       / var_definition
;
;-------------------------------------------------------------------------------

namespace name {
    public
    private
    protected

    class name (some.name) {
        var some.name name
        func a_name()()
    }

    ; note that the contents of the function is tested elsewhere.
    func name.a_name()() {}

    func name.create() {}

    func name.destroy {}

    var some.darn.name name = 123.123
}
