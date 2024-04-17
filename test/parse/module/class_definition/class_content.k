;-------------------------------------------------------------------------------
; Test for correct class content.
;-------------------------------------------------------------------------------

class name {
    public
    private
    protected
}

class name {
    func name2()()
    func name3(string, float, some.compound)()
    func name4()(some_name)
}

class name {
    var some.compound.name name1
    var float name2
    var const float name3
}

class name {
    public
    func name2()()
    func name3(string, float, some.compound)()
    private
    func name4()(some_name)
    var some.compound.name name1
    var float name2
    protected
    var const float name3
    func create(asdf, qwer)
    func destroy
}
