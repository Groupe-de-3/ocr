#include "least_squares.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "matrices.h"
#include "matrices_algebra.h"

/*! Solves a*x=b for x.
 *
 *  \param a The a Matrix.
 *  \param b The b Matrix.
 *  \param x The x Matrix which will be overwritten with the result.
 */
void la_solve_least_squares(
    Matrix(double) const a, Matrix(double) const b, Matrix(double) const x
) {
    Matrix(double) at = m_new(double, m_width(a), m_height(a));
    m_transpose(a, at);

    Matrix(double) at_inv = m_new(double, m_width(a), m_height(a));
    m_mul(at, a, at_inv);
    mal_inverse(at_inv);

    Matrix(double) final_a = m_new(double, m_width(a), m_height(a));
    m_mul(at_inv, at, final_a);

    m_mul(final_a, b, x);

    m_destroy(at);
    m_destroy(at_inv);
    m_destroy(final_a);
}
