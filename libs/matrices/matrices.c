#include "matrices.h"

#include <assert.h>
#include <stdalign.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t dimc;
    size_t dimv[MATRIX_MAX_DIMS];
} __attribute__((aligned(alignof(max_align_t)))) MatrixMetadata;

static MatrixMetadata *m_metadata(Matrix(void) m) {
    return (MatrixMetadata *)m - 1;
}

Matrix(void) m_create(size_t elem_size, size_t dimc, size_t dimv[]) {
    assert(dimc < MATRIX_MAX_DIMS);

    size_t total_size = 1;
    for (size_t i = 0; i < dimc; i++)
        total_size *= dimv[i];

    MatrixMetadata *matrix =
        malloc(sizeof(MatrixMetadata) + elem_size * total_size);
    memset(matrix, 0, sizeof(MatrixMetadata));

    matrix->dimc = dimc;
    memcpy(&matrix->dimv, dimv, dimc * sizeof(size_t));

    return (Matrix(void))(matrix + 1);
}
void m_destroy(void *m) {
    MatrixMetadata *allocated_ptr = (MatrixMetadata *)m - 1;
    free(allocated_ptr);
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

#define matrix_mul(name, type)                                                \
    void name(Matrix(type) a, Matrix(type) b, Matrix(type) out) {             \
        assert(m_width(a) == m_height(b));                                    \
        assert(m_height(a) == m_height(out));                                 \
        assert(m_width(b) == m_width(out));                                   \
                                                                              \
        size_t c1 = m_height(a);                                              \
        size_t c2 = m_width(b);                                               \
                                                                              \
        for (size_t row_i = 0; row_i < m_width(a); row_i++) {                 \
            for (size_t col_i = 0; col_i < m_height(b); col_i++) {            \
                type sum = 0.;                                                \
                for (size_t row_and_col = 0; row_and_col < c1; row_and_col++) \
                    sum += a[row_and_col + row_i * c1] *                      \
                           b[col_i + row_and_col * c2];                       \
                out[col_i + row_i * c2] = sum;                                \
            }                                                                 \
        }                                                                     \
    }
matrix_mul(m_imul, int) matrix_mul(m_fmul, double) matrix_mul(m_fmulf, float)

#define matrix_add(name, type, op)                                    \
    void name(Matrix(type) a, Matrix(type) out) {                     \
        MatrixMetadata *meta_a = m_metadata(a);                       \
        MatrixMetadata *meta_b = m_metadata(out);                     \
        assert(meta_a->dimc == meta_b->dimc);                         \
        for (size_t i = 0; i < meta_a->dimc; i++) {                   \
            assert(meta_a->dimv[i] == meta_b->dimv[i]);               \
        }                                                             \
                                                                      \
        size_t current_position[MATRIX_MAX_DIMS] = {0};               \
        for (;;) {                                                    \
            size_t index = meta_a->dimv[0];                           \
            for (ssize_t dim_i = (ssize_t)meta_a->dimc - 1;           \
                 dim_i > 0 /* Not processing last one*/; dim_i--) {   \
                index *= meta_a->dimv[dim_i];                         \
                index += current_position[dim_i];                     \
            }                                                         \
            out[index] = out[index] op a[index];                      \
                                                                      \
            current_position[meta_a->dimc - 1] = 0;                   \
            for (ssize_t dim_i = (ssize_t)meta_a->dimc - 1;           \
                 dim_i > 0 /* Not processing last one*/; dim_i--) {   \
                if (current_position[dim_i] == meta_a->dimv[dim_i]) { \
                    current_position[dim_i - 1] += 1;                 \
                    current_position[dim_i] = 0;                      \
                }                                                     \
            }                                                         \
        }                                                             \
    }
    // clang-format off
matrix_add(m_iadd, int, +)
matrix_add(m_fadd, double, +)
matrix_add(m_iaddf, float, +)

matrix_add(m_isub, int, -)
matrix_add(m_fsub, double, -)
matrix_add(m_isubf, float, -)
    // clang-format on
