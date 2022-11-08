#include "least_squares.h"
#include "matrices.h"

#include <math.h>
#include <stdio.h>
#include <assert.h>

#define LEARN_SPEED 0.00000000000001
#define THRESHOLD   0.0000001

/*
static Matrix(double) la_get_square_diff(Matrix(double) a, Matrix(double) b, Matrix(double) x) {
    Matrix(double) rslt = m_new(double, m_width(b), m_height(b));
    m_mul(a, x, rslt);
    for (size_t i = 0; i < m_length(x); i++) {
        rslt[i] = (rslt[i] - b[i]);
        rslt[i] *= rslt[i];
    }
    return rslt;
}
*/

static Matrix(double) la_get_square_diff_grad(Matrix(double) a, Matrix(double) b, Matrix(double) x) {
    Matrix(double) diff = m_new(double, m_width(b), m_height(b));
    //printf("%zu %zu x %zu %zu = %zu %zu\n", m_width(a), m_height(a), m_width(x), m_height(x), m_width(diff), m_height(diff));
    m_mul(a, x, diff);
    for (size_t i = 0; i < m_length(diff); i++)
        diff[i] = (diff[i] - b[i]);
      
    Matrix(double) grad = m_new(double, m_width(x), m_height(x));
    for (size_t j = 0; j < m_width(x); j++) {
        for (size_t i = 0; i < m_height(x); i++) {
            double sum = 0;
            for (size_t ai = 0; ai < m_height(a); ai++)
                sum += 2. *
                    m_get2(a, i, ai) *
                    m_get2(diff, j, ai);
            m_get2(grad, j, i) = sum;
        }
    }
    
    m_destroy(diff);
    return grad;
}

/*! Solves a*x=b for x.
 *
 *  \param a The a Matrix.
 *  \param b The b Matrix.
 *  \param x The x Matrix which will be overwritten with the result.
 */
void la_solve_least_squares(Matrix(double) const a, Matrix(double) const b, Matrix(double) const x) {
    for (size_t i = 0; i < m_length(x); i++)
        x[i] = 0;

    printf("A:\n");
    m_fprint(a);
    printf("X:\n");
    m_fprint(x);
    printf("B:\n");
    m_fprint(b);
    
    double learn_speed = LEARN_SPEED;

    bool found = false;
    for (size_t t = 0; t < 1000000 && !found; t++) {
        Matrix(double) grad = la_get_square_diff_grad(a, b, x);

        printf("Grad:\n");
        m_fprint(grad);

        learn_speed *= 2;
        for (size_t i = 0; i < m_length(x); i++)
            x[i] -= grad[i] * learn_speed;
        m_destroy(grad);

        printf("X:\n");
        m_fprint(x);
        
        Matrix(double) rslt = m_new(double, m_width(b), m_height(b));
        m_mul(a, x, rslt);
        
        printf("Rslt:\n");
        m_fprint(rslt);

        found = true;
        for (size_t i = 0; i < m_length(b); i++) {
            if (fabs(rslt[i] - b[i]) >= THRESHOLD)
                found = false;
        }
        
        m_destroy(rslt);
    }
}
