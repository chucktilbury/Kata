;-------------------------------------------------------------------------------
; Verify that module items are accepted.
;
;   module_item
;       = namespace_item
;       / import_statement
;       / start_definition
;
;-------------------------------------------------------------------------------

namespace name {}

public
private
protected

class name (some.name) {
    var some.name name
    func a_name()()
}

; note that the contents of the function is tested elsewhere.
func name.a_name()() {}

func create_name.create() {}

func destroy_name.destroy {}

var some.darn.name name = 123.123
var float name

; test this in the import test directory
; import empty as name

func start {}
