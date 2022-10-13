#include "tests.h"

#include <stdio.h>

#include "example_library.h"

const char *test_functions[] = {"test_add", "test_sub", NULL};

void test_add(void) {
    h_assert(add(5, 10) == 15);
    h_assert(add(10, 5) == 15);

    h_assert(add(-2, 3) == 1);
    h_assert(add(3, -2) == 1);

    h_assert(add(-3, 2) == -1);
    h_assert(add(2, -3) == -1);

    h_assert(add(10, 20) == 30);
    h_assert(add(20, 10) == 30);
}

void test_sub(void) {
    h_assert(sub(5, 10) == -5);
    h_assert(sub(10, 5) == 5);

    h_assert(sub(-2, 3) == -5);
    h_assert(sub(3, -2) == 5);

    h_assert(sub(-3, 2) == -5);
    h_assert(sub(2, -3) == 5);

    h_assert(sub(10, 20) == -10);
    h_assert(sub(20, 10) == 10);
}
