;-------------------------------------------------------------------------------
; Demonstrate nested for clause
;
;-------------------------------------------------------------------------------

func name ()() {

    for {
        for() {
            for ( name in something != 0 ) {
                for ( signed name in something != 0 ) {}
            }
        }
    }
}
