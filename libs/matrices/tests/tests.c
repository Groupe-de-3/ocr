#include "tests.h"

#include <stdio.h>
#include <assert.h>

#include "matrices.h"

const char *test_functions[] = {
    "test_mul2", "test_mul2_vec", "test_add2",
    "test_add2_transposed",
     NULL
};

void test_add2(void) {
    Matrix(int) a = m_new(int, 2, 2);
    // clang-format off
    m_copy(a, (int[]){
         8, -40,
        -3,  30,
    });
    // clang-format on

    Matrix(int) b = m_new(int, 2, 2);
    // clang-format off
    m_copy(b, (int[]){
        22, 43,
        -7,  3,
    });
    // clang-format on

    m_add(a, b);
    h_assert_int(m_get2(a, 0, 0), ==, 30);
    h_assert_int(m_get2(a, 1, 0), ==, 3);
    h_assert_int(m_get2(a, 0, 1), ==, -10);
    h_assert_int(m_get2(a, 1, 1), ==, 33);

    m_destroy(a);
    m_destroy(b);
}

void test_add2_transposed(void) {
    Matrix(int) a = m_new(int, 2, 1);
    // clang-format off
    m_copy(a, (int[]){
        125, -65,
    });
    // clang-format on

    Matrix(int) b = m_new(int, 1, 2);
    // clang-format off
    m_copy(b, (int[]){
        175,
        70,
    });
    // clang-format on

    m_add(a, b);
    h_assert_int(a[0], ==, 300);
    h_assert_int(a[1], ==, 5);

    m_destroy(a);
    m_destroy(b);
}

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
        7,   3,
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

void test_mul2_vec(void) {
    Matrix(int) a = m_new(int, 2, 2);
    // clang-format off
    m_copy(a, (int[]){
        5, 6,
        8, 9,
    });
    // clang-format on

    Matrix(int) b = m_new(int, 1, 2);
    // clang-format off
    m_copy(b, (int[]){
        -23,
         56,
    });
    // clang-format on

    Matrix(int) out = m_new(int, 1, 2);
    m_mul(a, b, out);

    h_assert_int(m_get2(out, 0, 0), ==, 221);
    h_assert_int(m_get2(out, 0, 1), ==, 320);

    m_destroy(a);
    m_destroy(b);
    m_destroy(out);
}
