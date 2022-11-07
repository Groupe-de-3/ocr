#include "least_squares.h"
#include "matrices.h"

#include <math.h>
#include <stdio.h>

#define LEARN_SPEED 0.05
#define THRESHOLD   0.00001

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
    m_mul(a, x, diff);
    for (size_t i = 0; i < m_length(x); i++)
        diff[i] = (diff[i] - b[i]);
      
    Matrix(double) grad = m_new(double, m_width(x), m_height(x));
    for (size_t j = 0; j < m_height(x); j++) {
        for (size_t i = 0; i < m_width(x); i++) {
            double sum = 0;
            for (size_t ai = 0; ai < m_height(a); ai++)
                sum += 2. * m_get2(a, ai, i) * m_get2(diff, ai, j);
            m_get2(grad, i, j) = sum;
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

    printf("X:\n");
    m_fprint(x);

    bool found = false;
    while (!found) {
        Matrix(double) grad = la_get_square_diff_grad(a, b, x);

        for (size_t i = 0; i < m_length(x); i++)
            x[i] -= grad[i] * LEARN_SPEED;
        m_destroy(grad);
        
        Matrix(double) rslt = m_new(double, m_width(x), m_height(x));
        m_mul(a, x, rslt);

        found = true;
        for (size_t i = 0; i < m_length(x); i++) {
            if (fabs(rslt[i] - b[i]) >= THRESHOLD)
                found = false;
        }
        
        m_destroy(rslt);
    }
}
