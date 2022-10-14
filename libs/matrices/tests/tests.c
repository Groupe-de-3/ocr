#include "tests.h"

#include <stdio.h>

#include "matrices.h"

const char *test_functions[] = {"test_mul2", NULL};

void test_mul2(void) {
    Matrix(int) id = m_new(int, 2, 2);
    // clang-format off
    m_copy(id, (int[]){
        1, 1,
        3, 1,
    });
    // clang-format on

    Matrix(int) m = m_new(int, 2, 2);
    // clang-format off
    m_copy(m, (int[]){
        22, 43,
        7, 3,
    });
    // clang-format on

    Matrix(int) out = m_new(int, 2, 2);
    m_mul(id, m, out);

    h_assert_int(m_get2(out, 0, 0), ==, 29);
    h_assert_int(m_get2(out, 1, 0), ==, 46);
    h_assert_int(m_get2(out, 0, 1), ==, 73);
    h_assert_int(m_get2(out, 1, 1), ==, 132);

    m_destroy(id);
    m_destroy(m);
    m_destroy(out);
}
