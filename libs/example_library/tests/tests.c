#include "tests.h"

#include <stdio.h>

#include "example_library.h"

void test_add(void) {
    h_assert(add(5, 10) == 15);
    h_assert(add(-2, 3) == 1);
    h_assert(add(-3, 2) == -1);
    h_assert(add(10, 20) == 30);
}
