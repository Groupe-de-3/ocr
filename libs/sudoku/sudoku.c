#include "sudoku.h"

#include <stdio.h>

void sudoku_print(char *board) {
    for (int i = 0; i < 9; ++i) {
        printf("------------------------------------\n");
        for (int j = 0; j < 9; ++j) {
            if (board[i * 9 + j] != -1) {
                printf("% 3i|", board[i * 9 + j]);
            }
            else {
                printf("   |");
            }
        }
        printf("\n");
    }
    printf("------------------------------------\n");
}

bool sudoku_solve(char *board) { }
