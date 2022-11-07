#include "tests.h"
#include "least_squares.h"

#include <stdio.h>

void test_least_squares(void) {
    Matrix(double) a = m_new(double, 3, 3);
    m_copy(a, (double[]) {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1,
    });
    Matrix(double) b = m_new(double, 3, 3);
    m_copy(b, (double[]) {
         32.0,   78.,  -30.,
        -120.0,  0.,    23.,
         9999.0, 1002., 99,
    });

    Matrix(double) x = m_new(double, 3, 3);
    la_solve_least_squares(a, b, x);
    
    Matrix(double) rslt = m_new(double, 3, 3);
    m_mul(a, x, rslt);
    
    for (size_t y = 0; y < m_height(x); y++) {
        for (size_t n = 0; n < m_width(x); n++) {
            printf("%f ", m_get2(x, n, y));
        }
        printf("\n");
    }
    
    for (size_t i = 0; i < m_length(rslt); i++)
        h_assert_double_eq(rslt[i], b[i], 0.00001);
}
