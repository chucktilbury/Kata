;-------------------------------------------------------------------------------
; Verify that namespaces can be nested.
;-------------------------------------------------------------------------------

namespace name {
    public

    class name (some.name) {
        public
        var some.name name
        func a_name()()
    }

    namespace name {
        public

        class name (some.name) {
            public
            var some.name name
            func a_name()()
        }
    }

    class name (some.name) {
        public
        var some.name name
        func a_name()()
    }

}
