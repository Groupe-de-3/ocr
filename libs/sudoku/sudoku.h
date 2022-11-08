#pragma once
#include <stdbool.h>
#define EMPTY_CELL 0

void sudoku_print(char *board);
bool checkBoardInterity(bool mustBeValid, char *board);
bool IsBoardValid(char *board);
bool IsSolved(char *board);
bool Solve(int i, int j, char *board);
