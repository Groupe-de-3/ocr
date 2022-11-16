#include <stdio.h>

#include "matrices_algebra.h"
#include "tests.h"

#define PRECISION 0.0000000001

void gauss_reduction_test1(void) {
    Matrix(double) a = m_new(double, 3, 3);
    m_copy(
        a,
        (double[]){
            1,
            0,
            0,
            0,
            2,
            0,
            0,
            0,
            1,
        }
    );

    Matrix(double) bonjour = m_new(double, 3, 3);
    m_copy(bonjour, a);

    Matrix(double) b = m_new(double, 3, 3);
    m_copy(
        b,
        (double[]){
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            1,
        }
    );

    mal_gauss_reduction(a, b);
    mal_backward_propagation(a, b);

    h_assert_double_eq(b[0], 1, PRECISION);
    h_assert_double_eq(b[1], 0, PRECISION);
    h_assert_double_eq(b[2], 0, PRECISION);

    h_assert_double_eq(b[3], 0, PRECISION);
    h_assert_double_eq(b[4], 0.5, PRECISION);
    h_assert_double_eq(b[5], 0, PRECISION);

    h_assert_double_eq(b[6], 0, PRECISION);
    h_assert_double_eq(b[7], 0, PRECISION);
    h_assert_double_eq(b[8], 1, PRECISION);
}
