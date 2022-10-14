#include <stdio.h>
#include "tests.h"
#include "matrices.h"

const char *test_functions[] = {
    "test_add", "test_sub",
    NULL
};

void test_add() {
    Matrix(int) id = m_new(int, 1, 2);
    m_copy(id, (int[]){
        1, 1,
        3, 1,
    });

    Matrix(int) m = m_new(int, 2, 1);
    m_copy(
        m,
        (int[]){
            22,
            43,
            7,
            3,
        }
    );

    Matrix(int) out = m_new(int, 2, 2);
    m_mul(id, m, out);

    for (size_t x = 0; x < m_width(out); x++)
        printf("------");
    printf("-\n");
    for (size_t y = 0; y < m_height(out); y++) {
        printf("|");
        for (size_t x = 0; x < m_width(out); x++) {
            printf(" %3i ", m_get2(out, x, y));
            printf("|");
        }
        printf("\n");
    }
    for (size_t x = 0; x < m_width(out); x++)
        printf("------");
    printf("-\n");

    m_destroy(id);
    m_destroy(m);
    m_destroy(out);
}
