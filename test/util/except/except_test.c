
#include "util.h"

// This enum would normally exist in the application. The first value does
// not really matter, but it has to be higher than the highest one that is
// used in the library.
typedef enum {
    // exception number zero is invalid
    EXCEPT_ONE = 100,
    EXCEPT_TWO,
    EXCEPT_THREE,
    EXCEPT_FOUR,
} exceptions;

void func4() {

    printf("before raising 0x8080\n");
    RAISE(0x8080, "this is exception 8080");
    printf("after raising 0x8080\n");
}

void func3() {

    printf("before calling 4\n");
    func4();
    printf("after calling 4\n");
}

void func2() {

    TRY {
        printf("before calling 3\n");
        func3();
        printf("after calling 3\n");
    }
    EXCEPT(EXCEPT_THREE) {
        printf("exception number %d is not expected\n", EXCEPTION_NUM);
        exit(1);
    }
    FINAL
}

void func1() {

    printf("before calling 2\n");
    func2();
    printf("after calling 2\n");
}

int main() {

    TRY {
        func1();
    }
    EXCEPT(EXCEPT_TWO) {
        printf("exception: %s\n", EXCEPTION_MSG);
    }
    /*
    ANY_EXCEPT() {
        printf("exception: 0x%04X: %s\n", EXCEPTION_NUM, EXCEPTION_MSG);
    }
    */
    FINAL

    printf("main() is about to exit.\n");

    return 0;
}
