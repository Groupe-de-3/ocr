#include "tests.h"

#include <assert.h>
#include <stdio.h>

#include "matrices.h"

const char *test_functions[] = {
    "test_mul2",
    "test_mul2_vec",
    "test_int_add2",
    "test_int_add2_transposed",
    "test_double_add2",
    "test_double_mul2",
    NULL};

void test_int_add2(void) {
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

void test_int_add2_transposed(void) {
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

void test_double_add2(void) {
    Matrix(double) a = m_new(double, 2, 2);
    // clang-format off
    m_copy(a, (double[]){
       -78.5,  48.2,
        498.2, 42.128,
    });
    // clang-format on

    Matrix(double) b = m_new(double, 2, 2);
    // clang-format off
    m_copy(b, (double[]){
         22.4894, 43.1123,
        -7.495,   3.186,
    });
    // clang-format on

    m_add(a, b);
    h_assert_double_eq(m_get2(a, 0, 0), -56.0106, 0.0001);
    h_assert_double_eq(m_get2(a, 1, 0), 91.3123, 0.0001);
    h_assert_double_eq(m_get2(a, 0, 1), 490.705, 0.001);
    h_assert_double_eq(m_get2(a, 1, 1), 45.314, 0.001);

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

void test_double_mul2(void) {
    Matrix(double) a = m_new(double, 2, 2);
    // clang-format off
    m_copy(a, (double[]){
        84.2,  45.156,
        49.5,  54.581,
    });
    // clang-format on

    Matrix(double) b = m_new(double, 2, 2);
    // clang-format off
    m_copy(b, (double[]){
        22.45, 42.69,
        798.5,  480.2,
    });
    // clang-format on

    Matrix(double) out = m_new(double, 2, 2);

    m_mul(a, b, out);
    h_assert_double_eq(m_get2(out, 0, 0), 37947.356, 0.001);
    h_assert_double_eq(m_get2(out, 1, 0), 25278.4092, 0.0001);
    h_assert_double_eq(m_get2(out, 0, 1), 44694.2035, 0.0001);
    h_assert_double_eq(m_get2(out, 1, 1), 28322.9512, 0.0001);

    m_destroy(a);
    m_destroy(b);
    m_destroy(out);
}
