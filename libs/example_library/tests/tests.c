#include <assert.h>

#include "example_library.h"

void test_add(void) {
    assert(add(5, 10) == 15);
    assert(add(-2, 3) == 1);
    assert(add(-3, 2) == -1);
    assert(add(10, 20) == 30);
}
