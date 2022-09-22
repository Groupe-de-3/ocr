#pragma once
#include <stddef.h>
#include <stdbool.h>

#define Matrix(t) t*
#define MATRIX_MAX_DIMS 3

#define m_new(type, dimc, dimv) (Matrix(type))m_create(sizeof(type), dimc, dimv)

Matrix(void) m_create(size_t elem_size, size_t dimc, size_t dimv[]);
void m_destroy(Matrix(void) m);

size_t m_comp_dims(Matrix(void) m);

size_t m_length(Matrix(void) m);
size_t m_width(Matrix(void) m);
size_t m_height(Matrix(void) m);

#define m_get2(m, type, x, y) m[m_height(m)*y + x]

#define m_mul(A, B, out) _Generic((A), \
    Matrix(int):    m_imul,  \
    Matrix(double): m_fmul,  \
    Matrix(float):  m_fmulf  \
)(A, B, out)
void m_imul(Matrix(int) a, Matrix(int) b, Matrix(int) out);
void m_fmul(Matrix(double) a, Matrix(double) b, Matrix(double) out);
void m_fmulf(Matrix(float) a, Matrix(float) b, Matrix(float) out);

#define m_add(A, B) _Generic((A), \
    Matrix(int):    m_imul,  \
    Matrix(double): m_fmul,  \
    Matrix(float):  m_fmulf  \
)(A, B)
void m_iadd(Matrix(int) a, Matrix(int) out);
void m_fadd(Matrix(double) a, Matrix(double) out);
void m_faddf(Matrix(float) a, Matrix(float) out);

#define m_sub(A, B) _Generic((A), \
    Matrix(int):    m_imul,  \
    Matrix(double): m_fmul,  \
    Matrix(float):  m_fmulf  \
)(A, B)
void m_isub(Matrix(int) a, Matrix(int) out);
void m_fsub(Matrix(double) a, Matrix(double) out);
void m_fsubf(Matrix(float) a, Matrix(float) out);
