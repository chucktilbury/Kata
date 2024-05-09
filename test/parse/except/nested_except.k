;-------------------------------------------------------------------------------
; nested in except clause
;-------------------------------------------------------------------------------

func name()() {

    try {
        xyz = 123
    }
    except(name1, name2) {
        func_name1(name1)()
        try {
            xyz = 123
        }
        except(name3, name4) {
            func_name2(name5)()
        }
        except(name6, name7) {
            func_name3(name8)()
            try {
                ghj = 123
            }
            except(name9, name10) {
                func_name4(name11)()
            }
            except(name12, name13) {
                func_name5(name14)()
            }
            final(name15) {
                ert = 321
            }
        }
        final(name16) {
            uio = 321
        }
    }
}