;-------------------------------------------------------------------------------
; exception clause params are not simple symbols (first)
;-------------------------------------------------------------------------------

func name1()() {

    try {
        name2 = "string"
    }
    except(compound.name3, name4) {
        name5 = 123
    }
}