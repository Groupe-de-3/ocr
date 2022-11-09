#pragma once

#include "matrices.h"

/*! Solves a*x=b for x.
 *
 *  \param a The a Matrix.
 *  \param b The b Matrix.
 *  \param x The x Matrix which will be overwritten with the result.
 */
void la_solve_least_squares(
    Matrix(double) a, Matrix(double) b, Matrix(double) x
);
