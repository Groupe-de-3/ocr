#pragma once
#include <stdbool.h>
#include <stddef.h>

#include "utils_countargs.h"

/*! \file matrices.h
 *
 *  \brief This header file contains every function and type definition of the
 * matrices library.
 *
 *  The matrices library handle eveything matrix-related.
 */

/*! \brief Macro for the type of a Matrix with the type `t` inside.
 *
 *  \param t What type is the content of the Matrix.
 *  \return A type for holding a Matrix.
 */
#define Matrix(t) t *
/*! \brief The maximum number of dimensions a matrix can have.
 */
#define MATRIX_MAX_DIMS 3

/*! \brief Convinient macro for calling m_create.
 *
 *  This create a new matrix.
 *  Don't forget to call #m_destroy after usage!!!!
 *
 *  \par Example
 *  \parblock \code
 *  // Creates a new 2 by 3 matrix of ints
 *  Matrix(int) m = m_new(int, 2, 3);
 *  assert(m_length(m) == 6);
 *  assert(m_width(m) == 2);
 *  assert(m_height(m) == 3);
 *  m_destroy(m);
 *  \endcode \endparblock
 *
 *  \param type The type of the Matrix's content.
 *  \param ... List of the length of each dimensions of the matrix.
 *  \return A new Matrix with the specified sizes and type.
 */
#define m_new(type, ...)                                                    \
    (Matrix(type)) m_create(                                                \
        sizeof(type), utils_countargs(__VA_ARGS__), (size_t[]){__VA_ARGS__} \
    )

/*! \brief Creates a new matrix, prefer using #m_new instead.
 *
 *  Don't forget to call #m_destroy after usage!!!!
 *
 *  \param elem_size The size of a single matrix element in bytes.
 *  \param dimc The size of the \a dimv array.
 *  \param dimv Array of the sizes of the Matrix's dimensions.
 *  \return A new Matrix with the specified sizes and content size.
 */
Matrix(void) m_create(size_t elem_size, size_t dimc, size_t dimv[]);
/*! \brief Frees the memory used by a matrix.
 *
 *  Must be called when the matrix isn't needed anymore. The matrix cannot
 *  be used after this.
 *
 *  \param m The matrix to destroy.
 */
void m_destroy(Matrix(void) m);

size_t  m_elem_size(Matrix(void) m);
size_t  m_dimc(Matrix(void) m);
size_t *m_dimv(Matrix(void) m);

void m_fprint(Matrix(double) m);

/*! \brief Get the length of a matrix.
 *
 *  Instead of returning the size of a single dimension, this computes the
 *  number of elements across all dimensions.
 *
 *  \par Example
 *  \parblock \code
 *  // Creates a new 2 by 3 matrix of ints
 *  Matrix(int) m1 = m_new(int, 2, 3);
 *  assert(m_length(m1) == 6);
 *  m_destroy(m1);
 *
 *  // Creates a new 4 by 2 b 10 matrix of ints
 *  Matrix(int) m2 = m_new(int, 4, 2, 10);
 *  assert(m_length(m2) == 80);
 *  m_destroy(m2);
 *  \endcode \endparblock
 *
 *  \param m The matrix of which to get the length.
 *  \return The total amount of element in the matrix.
 */
size_t m_length(Matrix(void) m);
/*! \brief Get the width of a matrix.
 *
 *  The width is the first dimension of the matrix.
 *  \par Safety
 *  The passed matrix must have at least one dimension.
 *
 *  \param m The matrix of which to get the width.
 *  \return The size of the matrix's first dimension.
 */
size_t m_width(Matrix(void) m);
/*! \brief Get the height of a matrix.
 *
 *  The height is the second dimension of the matrix.
 *  \par Safety
 *  The passed matrix must have at least two dimension.
 *
 *  \par Example
 *  \parblock \code
 *  // Creates a new 2 by 3 matrix of ints
 *  Matrix(int) m1 = m_new(int, 2, 3);
 *  assert(m_width(m2) == 2);
 *  assert(m_height(m2) == 3);
 *  m_destroy(m1);
 *
 *  // Creates a new 4 by 2 b 10 matrix of ints
 *  Matrix(int) m2 = m_new(int, 4, 2, 10);
 *  assert(m_width(m2) == 4);
 *  assert(m_height(m2) == 3);
 *  m_destroy(m2);
 *  \endcode \endparblock
 *
 *  \param m The matrix of which to get the height.
 *  \return The size of the matrix's second dimension.
 */
size_t m_height(Matrix(void) m);

void m_copy(Matrix(void) destination, void *data);
bool m_dim_eq(Matrix(void) a, Matrix(void) b);
bool m_eq(Matrix(void) a, Matrix(void) b);

/*! \brief Convinient macro for accessing elements of a 2d matrix.
 *
 *  \par Example
 *  \parblock \code
 *  Matrix(int) m = m_new(int, 3, 3);
 *  for (int y = 0; y < 3; y++)
 *      for (int x = 0; x < 3; x++)
 *          m_get2(m, x, y) = x + y;
 *
 *  assert(m[0] == 3);
 *  assert(m_get2(m, 0, 0) == 0);
 *
 *  assert(m[2 * 3 + 1] == 3);
 *  assert(m_get2(m, 1, 2) == 3);
 *  m_destroy(m);
 *  \endcode \endparblock
 *
 *  \param m The matrix to access.
 *  \param x The position in the first dimension.
 *  \param y The position in the second dimension.
 *  \return The value of the element at the given position.
 */
#define m_get2(m, x, y) (m)[m_width((m)) * (y) + (x)]

/*! \brief Transposes the in matrix into the out matrix.
 *
 *  \param in The 2D matrix to transpose.
 *  \param out The output of the transpose operation, its height and width must
 * be equal respectively to the width and height of the in matrix.
 */
void m_transpose(Matrix(double) in, Matrix(double) out);

/*! \brief Computes the hadamard product of two matrices.
 *
 *  \param[in] a The First Matrix.
 *  \param[in,out] out The second Matrix, also the destination of the
 * computation.
 */
void m_hadamard_product(Matrix(double) a, Matrix(double) out);

/*! \brief Convinient macro for using functions in the form m_xmul.
 *
 *  All matrices must be of the same type and respect the following:
 *  \code
 *  assert(m_width(a) == m_height(b));
 *  assert(m_height(a) == m_height(out));
 *  assert(m_width(b) == m_width(out));
 *  \endcode
 *
 *  Calling this macro will redirect to the correct function:
 *  - For matrices of ints: #m_imul
 *  - For matrices of doubles: #m_fmul
 *  - For matrices of floats: #m_fmulf
 *
 *  \par Example
 *  \parblock \code
 *  // Creates a new 3 by 3 matrix of ints
 *  Matrix(int) m1 = m_new(int, 3, 3);
 *  m_copy(m1, (int[]) {
 *      1, 2, 3,
 *      4, 5, 6,
 *      7, 8, 9
 *  });
 *
 *  Matrix(int) m2 = m_new(int, 3, 3);
 *  m_copy(m2, (int[]) {
 *      5, 3, 1,
 *      7, 3, 9,
 *      9, 1, 4
 *  });
 *
 *  Matrix(int) out = m_new(int, 3, 3);
 *  m_mul(m1, m2, out);
 *
 *  Matrix(int) excpected = m_new(int, 3, 3);
 *  m_copy(excpected, (int[]) {
 *      f, f, f,
 *      f, f, f,
 *      f, f, f
 *  });
 *
 *  assert(m_eq(out, excpected));
 *
 *  m_destroy(m1);
 *  m_destroy(m2);
 *  m_destroy(out);
 *  m_destroy(excpected);
 *  \endcode \endparblock
 *
 *  \param[in] A The first matrix.
 *  \param[in] B The second matrix.
 *  \param[out] out The matrix where to save the result.
 */
#define m_mul(A, B, out)              \
    _Generic((A), Matrix(int)         \
             : m_imul, Matrix(double) \
             : m_fmul, Matrix(float)  \
             : m_fmulf)(A, B, out)

/*! \brief Multiplies the given int matrices of together, see #m_mul.
 *
 *  \param[in] a The first matrix.
 *  \param[in] b The second matrix.
 *  \param[out] out The matrix where to save the result.
 */
void m_imul(Matrix(int) a, Matrix(int) b, Matrix(int) out);
/*! \brief Multiplies the given double matrices of together, see #m_mul.
 *
 *  \param[in] a The first matrix.
 *  \param[in] b The second matrix.
 *  \param[out] out The matrix where to save the result.
 */
void m_fmul(Matrix(double) a, Matrix(double) b, Matrix(double) out);
/*! \brief Multiplies the given float matrices of together, see #m_mul.
 *
 *  \param[in] a The first matrix.
 *  \param[in] b The second matrix.
 *  \param[out] out The matrix where to save the result.
 */
void m_fmulf(Matrix(float) a, Matrix(float) b, Matrix(float) out);

/*! \brief Convinient macro for using functions in the form m_xadd.
 *
 *  All matrices must be of the same type and of the same size.
 *
 *  Calling this macro will redirect to the correct function:
 *  - For matrices of ints: #m_iadd
 *  - For matrices of doubles: #m_fadd
 *  - For matrices of floats: #m_faddf
 *
 *  \param[in] A The first matrix.
 *  \param[out] B The second matrix, also the output destination.
 */
#define m_add(A, B)                   \
    _Generic((A), Matrix(int)         \
             : m_iadd, Matrix(double) \
             : m_fadd, Matrix(float)  \
             : m_faddf)(A, B)

/*! \brief Adds the given int matrices of together, see #m_add.
 *
 *  \param[in] a The first matrix.
 *  \param[out] out The matrix where to save the result.
 */
void m_iadd(Matrix(int) a, Matrix(int) out);
/*! \brief Adds the given doubles matrices of together, see #m_add.
 *
 *  \param[in] a The first matrix.
 *  \param[out] out The matrix where to save the result.
 */
void m_fadd(Matrix(double) a, Matrix(double) out);
/*! \brief Adds the given floats matrices of together, see #m_add.
 *
 *  \param[in] a The first matrix.
 *  \param[out] out The matrix where to save the result.
 */
void m_faddf(Matrix(float) a, Matrix(float) out);

/*! \brief Convinient macro for using functions in the form m_xsub.
 *
 *  All matrices must be of the same type and of the same size.
 *
 *  Calling this macro will redirect to the correct function:
 *  - For matrices of ints: #m_isub
 *  - For matrices of doubles: #m_fsub
 *  - For matrices of floats: #m_fsubf
 *
 *  \param[in] A The first matrix.
 *  \param[out] B The second matrix, also the output destination.
 */
#define m_sub(A, B)                   \
    _Generic((A), Matrix(int)         \
             : m_imul, Matrix(double) \
             : m_fmul, Matrix(float)  \
             : m_fmulf)(A, B)

/*! \brief Substract the first matrix from the second matrix, see #m_sub.
 *
 *  \param[in] a The first matrix.
 *  \param[out] out The second matrix, also the output destination.
 */
void m_isub(Matrix(int) a, Matrix(int) out);
/*! \brief Substract the first matrix from the second matrix, see #m_sub.
 *
 *  \param[in] a The first matrix.
 *  \param[out] out The matrix where to save the result.
 */
void m_fsub(Matrix(double) a, Matrix(double) out);
/*! \brief Substract the first matrix from the second matrix, see #m_sub.
 *
 *  \param[in] a The first matrix.
 *  \param[out] out The matrix where to save the result.
 */
void m_fsubf(Matrix(float) a, Matrix(float) out);
