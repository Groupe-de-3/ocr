#include "matrices.h"

#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t dimc;
    size_t dimv[MATRIX_MAX_DIMS];
    size_t elem_size;
} __attribute__((aligned(alignof(max_align_t)))) MatrixMetadata;

static MatrixMetadata *m_metadata(Matrix(void) m) {
    return (MatrixMetadata *)m - 1;
}

Matrix(void) m_create(size_t elem_size, size_t dimc, size_t dimv[]) {
    assert(dimc <= MATRIX_MAX_DIMS);

    size_t total_size = 1;
    for (size_t i = 0; i < dimc; i++)
        total_size *= dimv[i];

    MatrixMetadata *matrix =
        malloc(sizeof(MatrixMetadata) + elem_size * total_size);
    memset(matrix, 0, sizeof(MatrixMetadata));

    matrix->dimc = dimc;
    memcpy(&matrix->dimv, dimv, dimc * sizeof(size_t));
    matrix->elem_size = elem_size;

    return (Matrix(void))(matrix + 1);
}
void m_destroy(void *m) {
    MatrixMetadata *allocated_ptr = (MatrixMetadata *)m - 1;
    free(allocated_ptr);
}

size_t m_elem_size(Matrix(void) m) {
    return m_metadata(m)->elem_size;
}
size_t m_dimc(Matrix(void) m) {
    return m_metadata(m)->dimc;
}
size_t *m_dimv(Matrix(void) m) {
    return m_metadata(m)->dimv;
}

size_t m_length(Matrix(void) m) {
    MatrixMetadata *metadata = m_metadata(m);
    if (metadata->dimc == 0)
        return 0;
    size_t total_size = 1;
    for (size_t i = 0; i < metadata->dimc; i++)
        total_size *= metadata->dimv[i];
    return total_size;
}
size_t m_width(Matrix(void) m) {
    MatrixMetadata *metadata = m_metadata(m);
    assert(metadata->dimc >= 1);
    return metadata->dimv[0];
}
size_t m_height(Matrix(void) m) {
    MatrixMetadata *metadata = m_metadata(m);
    assert(metadata->dimc >= 2);
    return metadata->dimv[1];
}

void m_copy(Matrix(void) destination, void *data) {
    memcpy(destination, data, m_elem_size(destination) * m_length(destination));
}
bool m_dim_eq(Matrix(void) a, Matrix(void) b) {
    if (m_dimc(a) != m_dimc(b))
        return false;

    for (size_t i = 0; i < m_dimc(a); i++) {
        if (m_dimv(a)[i] != m_dimv(b)[i])
            return false;
    }

    return true;
}
bool m_eq(Matrix(void) a, Matrix(void) b) {
    assert(m_dim_eq(a, b));
    assert(m_elem_size(a) == m_elem_size(b));
    char *av = a;
    char *bv = b;

    for (size_t i = 0; i < m_elem_size(a) * m_length(a); i++) {
        if (av[i] != bv[i])
            return false;
    }

    return true;
}

#define matrix_mul(name, type)                                                \
    void name(Matrix(type) a, Matrix(type) b, Matrix(type) out) {             \
        assert(m_dimc(a) >= 2);                                               \
        assert(m_dimc(b) >= 2);                                               \
        assert(m_dimc(out) >= 2);                                             \
                                                                              \
        size_t c1 = m_height(a);                                              \
        size_t c2 = m_width(b);                                               \
                                                                              \
        assert(m_width(a) == m_height(b));                                    \
        assert(c1 == m_height(out));                                          \
        assert(c2 == m_width(out));                                           \
                                                                              \
        for (size_t row_i = 0; row_i < c1; row_i++) {                         \
            for (size_t col_i = 0; col_i < c2; col_i++) {                     \
                type sum = 0.;                                                \
                for (size_t row_and_col = 0; row_and_col < c1; row_and_col++) \
                    sum += m_get2(a, row_and_col, row_i) *                    \
                           m_get2(b, col_i, row_and_col);                     \
                m_get2(out, col_i, row_i) = sum;                              \
            }                                                                 \
        }                                                                     \
    }
matrix_mul(m_imul, int) matrix_mul(m_fmul, double) matrix_mul(m_fmulf, float)

#define matrix_add(name, type, op)                           \
    void name(Matrix(type) out, Matrix(type) b) {            \
        size_t total_length = m_length(out);                 \
        /* Ensures both matrices have the same total size */ \
        assert(total_length == m_length(b));                 \
        for (size_t i = 0; i < total_length; i++) {          \
            out[i] += b[i];                                  \
        }                                                    \
    }
    // clang-format off
matrix_add(m_iadd, int, +)
matrix_add(m_fadd, double, +)
matrix_add(m_iaddf, float, +)

matrix_add(m_isub, int, -)
matrix_add(m_fsub, double, -)
matrix_add(m_isubf, float, -)
    // clang-format on
