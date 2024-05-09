;-------------------------------------------------------------------------------
; nested in try clause
;-------------------------------------------------------------------------------

func name()() {

    try {
        name26 = 123
        try {
            name23 = 123
        }
        except(name5, name6) {
            func_name19(name7)()
        }
        except(name8, name9) {
            func_name20(name10)()
            try {
                name24 = 4556
            }
            except(name10, name11) {
                func_name21(name12)()
            }
            except(name13, name14) {
                func_name22(name15)()
            }
            final(name16) {
                name25 = 321
            }
        }
        final(name17) {
            name26 = 789
        }
    }
    except(name1, name2) {
        func_name18(name3)()
    }
    final(name4) {
        name27()()
    }
}