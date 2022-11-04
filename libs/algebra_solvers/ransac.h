#pragma once

#include "matrices.h"

/*! \brief Solves a*b=c for b using the ransac algorithm and saves the result in b.
 *
 *  Multiplication of the a matrices with the b matrix must make a matrix
 *  of the same size of c.
 *
 *  \param[in] a The a matrix, see above for size requirements.
 *  \param[in, out] b The b matrix which will be updated, see above for size requirements.
 *  \param[in] c The c matrix, see above for size requirements.
 */
void as_solve_ransac(Matrix(double) a, Matrix(double) b, Matrix(double) c);