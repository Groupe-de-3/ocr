#include <stddef.h>
#include <stdio.h>
#include "matrices.h"

#define VALUE 5

int main() {
    Matrix(int) id = m_new(int, 2, 2);
    m_copy(id, (int[]) {
        1, 0,
        0, 1
    });

    Matrix(int) m = m_new(int, 2, 2);
    m_copy(m, (int[]) {
        22, 10,
        7,  10,
    });

    Matrix(int) out = m_new(int, 1, 2);
    m_mul(id, m, out);

    for (size_t y = 0; y < m_height(out); y++) {
        for (size_t x = 0; x < m_width(out); x++) {
            printf("%i ", m_get2(out, x, y));
        }
        printf("\n");
    }

    m_destroy(id);
    m_destroy(m);
    m_destroy(out);
}
