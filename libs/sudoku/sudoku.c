#include "sudoku.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <vec.h>

void sudoku_print(char *board) {
    for (int i = 0; i < 9; ++i) {
        printf("------------------------------------\n");
        for (int j = 0; j < 9; ++j) {
            if (board[i * 9 + j] != 0) {
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

bool contains(char *list, int x) {
    for (size_t i = 0; i < vec_size(list); ++i) {
        if (list[i] == x) {
            return true;
        }
    }
    return false;
}

bool checkBoardInterity(bool mustBeValid, char *board) {
    for (int blockI = 0; blockI < 3; ++blockI) {
        for (int blockJ = 0; blockJ < 3; ++blockJ) {
            unsigned int foundBitMap = 0;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    unsigned int bit =
                        1u << board[(blockI * 3 + i) * 9 + (blockJ * 3 + j)];
                    if ((foundBitMap & bit) != 0 && bit != 1)
                        return false;
                    foundBitMap |= bit;
                }
            }
            if (mustBeValid && foundBitMap != 0x3FE)
                return false;
        }
    }

    for (int i = 0; i < 9; i++) {
        unsigned int foundBitMap = 0;
        for (int j = 0; j < 9; j++) {
            unsigned int bit = 1u << board[i * 9 + j];
            if ((foundBitMap & bit) != 0 && bit != 1)
                return false;
            foundBitMap |= bit;
        }
        if (mustBeValid && foundBitMap != 0x3FE)
            return false;
    }
    for (int j = 0; j < 9; j++) {
        unsigned int foundBitMap = 0;
        for (int i = 0; i < 9; i++) {
            unsigned int bit = 1u << board[i * 9 + j];
            if ((foundBitMap & bit) != 0 && bit != 1)
                return false;
            foundBitMap |= bit;
        }
        if (mustBeValid && foundBitMap != 0x3FE)
            return false;
    }

    return true;
}

bool IsBoardValid(char *board) {
    return checkBoardInterity(false, board);
}

bool IsSolved(char *board) {
    return checkBoardInterity(true, board);
}

bool Solve(int i, int j, char *board) {
    while (j < 9 && board[i * 9 + j] != 0) {
        i++;
        if (i >= 9) {
            i = 0;
            j += 1;
        }
    }

    if (j >= 9) {
        return IsSolved(board);
    }
    for (char k = 1; k <= 9; ++k) {
        board[i * 9 + j] = k;
        if (!IsBoardValid(board)) {
            continue;
        }
        if (Solve(i, j, board)) {
            return true;
        }
    }
    board[i * 9 + j] = 0;
    return false;
}
