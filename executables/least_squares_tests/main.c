#include <stdio.h>

#include "least_squares.h"

int main() {
    Matrix(double) a = m_new(double, 3, 3);
    m_copy(a, (double[]) {
        2, 1, 1,
        1, 2, 1,
        1, 1, 2,
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
    
    printf("FINAL:\n");
    m_fprint(rslt);

    printf("Solution:\n");
    m_fprint(x);

    m_destroy(a);
    m_destroy(b);
    m_destroy(x);
    m_destroy(rslt);
}
