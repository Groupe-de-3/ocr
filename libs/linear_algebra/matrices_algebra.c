#include "matrices_algebra.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "utils_mem.h"

void utils_memswap(void *from, void *to, size_t length) {
    char *fromb = (char *)from;
    char *tob   = (char *)to;
    for (size_t i = 0; i < length; i++) {
        char temp = *fromb;
        *fromb    = *tob;
        *tob      = temp;
        fromb++;
        tob++;
    }
}

void mal_gauss_reduction(Matrix(double) a, Matrix(double) b) {
    assert(m_width(a) == m_width(b));
    assert(m_height(a) == m_height(b));

    size_t pivot_x = 0;
    size_t pivot_y = 0;
    while (pivot_x < m_width(a) && pivot_y < m_height(a)) {
        /*
        size_t max_y = 0;
        for (size_t y = pivot_y; y < m_height(a); y++)
            if (m_get2(a, pivot_x, y) > m_get2(a, pivot_x, max_y))
                max_y = y;

        // Swaping the first row with the max row
        if (m_get2(a, pivot_x, max_y) == 0) {
            pivot_x++;
            continue;
        }

        if (max_y != pivot_y) {
            utils_memswap(
                &m_get2(a, 0, max_y), &m_get2(a, 0, pivot_y),
                sizeof(double) * m_width(a)
            );
            utils_memswap(
                &m_get2(b, 0, max_y), &m_get2(b, 0, pivot_y),
                sizeof(double) * m_width(b)
            );
        }
        */

        // For each row we try to make the pivot = 1
        for (size_t y = pivot_y + 1; y < m_height(a); y++) {
            double factor = m_get2(a, pivot_x, y) / m_get2(a, pivot_x, pivot_y);
            m_get2(a, pivot_x, y) = 0.;
            for (size_t nx = pivot_x + 1; nx < m_width(a); nx++)
                m_get2(a, nx, y) -= m_get2(a, nx, pivot_y) * factor;
            for (size_t nx = 0; nx < m_width(b); nx++)
                m_get2(b, nx, y) -= m_get2(b, nx, pivot_y) * factor;
        }

        pivot_x++;
        pivot_y++;
    }
}

void mal_backward_propagation(Matrix(double) a, Matrix(double) b) {
    assert(m_width(a) == m_width(b));
    assert(m_height(a) == m_height(b));
    assert(m_width(a) == m_height(b));
    size_t w = m_width(a);
    size_t h = m_height(a);

    for (size_t npivot = 0; npivot < h; npivot++) {
        size_t pivot = h - npivot - 1;

        // Normalize the row (0 and only one 1)
        for (size_t x = 0; x < w; x++)
            m_get2(b, x, pivot) /= m_get2(a, pivot, pivot);
        m_get2(a, pivot, pivot) = 1.;

        // Set to zero the whole column
        for (size_t y = 0; y < pivot; y++) {
            for (size_t x = 0; x < w; x++)
                m_get2(b, x, y) -= m_get2(b, x, pivot) * m_get2(a, pivot, y);
            m_get2(a, pivot, y) = 0.;
        }
    }
}

static void mal_inverse_2by2(Matrix(double) m) {
    assert(m_dimc(m) == 2);
    assert(m_width(m) == m_height(m));
    assert(m_width(m) == 2);

    double determinent =
        m_get2(m, 0, 0) * m_get2(m, 1, 1) - m_get2(m, 1, 0) * m_get2(m, 0, 1);
    assert(determinent != 0.);
    double det_1 = 1. / determinent;

    double temp     = m_get2(m, 0, 0);
    m_get2(m, 0, 0) = m_get2(m, 1, 1);
    m_get2(m, 1, 1) = temp;
    m_get2(m, 0, 1) *= -1;
    m_get2(m, 1, 0) *= -1;

    for (size_t i = 0; i < 4; i++)
        m[i] *= det_1;
}

static void mal_inverse_3by3(Matrix(double) m) {
    double a = m[0], b = m[1], c = m[2], d = m[3], e = m[4], f = m[5], g = m[6],
           h = m[7], i = m[8];
    double a2 = (e * i - f * g), b2 = -(d * i - f * g), c2 = (d * h - e * g);
    double d2 = -(b * i - c * h), e2 = (a * i - c * g), f2 = -(a * h - b * g);
    double g2 = (b * f - c * e), h2 = -(a * f - c * d), i2 = (a * e - b * d);

    double determinent = a * a2 + b * b2 + c * c2;
    assert(determinent != 0.);

    double det_1 = 1. / determinent;
    memcpy(
        m,
        (double[]){
            a2 * det_1,
            d2 * det_1,
            g2 * det_1,
            b2 * det_1,
            e2 * det_1,
            h2 * det_1,
            c2 * det_1,
            f2 * det_1,
            i2 * det_1,
        },
        sizeof(double) * 3 * 3
    );
}

static void mal_inverse_NbyN(Matrix(double) m) {
    assert(m_width(m) == m_height(m));

    Matrix(double) normal = m_new(double, m_width(m), m_height(m));
    m_copy(normal, m);

    // Setting m to identity matrix
    memset(m, 0, sizeof(double) * m_length(m));
    for (size_t i = 0; i < m_width(m); i++)
        m_get2(m, i, i) = 1.;

    mal_gauss_reduction(normal, m);
    mal_backward_propagation(normal, m);

    m_destroy(normal);
}

/*
Not working implementation of
https://www.hindawi.com/journals/ijmms/2012/134653/#EEq3.1

static void mal_inverse_NbyN(Matrix(double) m) {
    assert(m_width(m) == m_height(m));

    Matrix(double) id = m_new(double, m_width(m), m_height(m));
    memset(id, 0, sizeof(double) * m_length(m));
    for (size_t i = 0; i < m_width(m); i++)
        m_get2(id, i, i) = 1;

    Matrix(double) rslt = m_new(double, m_width(m), m_height(m));
    m_transpose(m, rslt);
    double norm_1_inf = mal_matrix_norm_1(m) * mal_matrix_norm_inf(m);
    for (size_t i = 0; i < m_length(m); i++)
        rslt[i] *= norm_1_inf;

    Matrix(double) rslt_a = m_new(double, m_width(m), m_height(m));

    Matrix(double) rslt_1a = m_new(double, m_width(m), m_height(m));
    Matrix(double) rslt_1b = m_new(double, m_width(m), m_height(m));

    for (size_t n = 0; n < 10; n++) {
        m_mul(m, rslt, rslt_a);
        m_copy(rslt_1a, rslt_a);

        printf("RSLT:\n");
        m_fprint(rslt);
        printf("RSLT A:\n");
        m_fprint(rslt_a);
        printf("RSLT 1A:\n");
        m_fprint(rslt_1a);
        printf("RSLT 1B:\n");
        m_fprint(rslt_1b);

        for (size_t i = 0; i < m_width(m); i++)
            m_get2(rslt_1a, i, i) -= 15;
        m_mul(rslt_a, rslt_1a, rslt_1b);

        for (size_t i = 0; i < m_width(m); i++)
            m_get2(rslt_1b, i, i) += 93;
        m_mul(rslt_a, rslt_1b, rslt_1a);

        for (size_t i = 0; i < m_width(m); i++)
            m_get2(rslt_1a, i, i) -= 315;
        m_mul(rslt_a, rslt_1a, rslt_1b);

        for (size_t i = 0; i < m_width(m); i++)
            m_get2(rslt_1b, i, i) += 651;
        m_mul(rslt_a, rslt_1b, rslt_1a);

        for (size_t i = 0; i < m_width(m); i++)
            m_get2(rslt_1a, i, i) -= 861;
        m_mul(rslt_a, rslt_1a, rslt_1b);

        for (size_t i = 0; i < m_width(m); i++)
            m_get2(rslt_1b, i, i) += 735;
        m_mul(rslt_a, rslt_1b, rslt_1a);

        for (size_t i = 0; i < m_width(m); i++)
            m_get2(rslt_1a, i, i) -= 393;
        m_mul(rslt_a, rslt_1a, rslt_1b);

        for (size_t i = 0; i < m_width(m); i++)
            m_get2(rslt_1b, i, i) += 120;
        m_mul(rslt_a, rslt_1b, rslt_1a);

        m_mul(rslt, rslt_1a, rslt_1b);
        for (size_t i = 0; i < m_length(rslt); i++)
            rslt[i] = rslt_1b[i] / 16.;
    }

    m_copy(m, rslt);

    m_destroy(rslt);
    m_destroy(rslt_a);
    m_destroy(rslt_1a);
    m_destroy(rslt_1b);
}
*/

void mal_inverse(Matrix(double) m) {
    assert(m_dimc(m) == 2);

    size_t w = m_width(m);
    assert(w == m_height(m));

    if (w == 2)
        mal_inverse_2by2(m);
    else if (w == 3)
        mal_inverse_3by3(m);
    else
        mal_inverse_NbyN(m);
}
