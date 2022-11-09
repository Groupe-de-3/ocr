#include <stdio.h>

#include "matrices_algebra.h"
#include "tests.h"

void gauss_reduction_tests(void) {
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

    printf("\n_-_-_- Done reduction\n");

    printf("A:\n");
    m_fprint(a);
    printf("B:\n");
    m_fprint(b);

    printf("\n_-_-_-_-_-_-_-_-_-_-\n");

    mal_backward_propagation(a, b);

    printf("A:\n");
    m_fprint(a);
    printf("B:\n");
    m_fprint(b);

    printf("\n\n_-_-_- Done\n");

    h_assert(false);
}
