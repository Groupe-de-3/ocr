#pragma once
#include <stdbool.h>
#define EMPTY_CELL 0

void sudoku_print(char *board);
bool check_board_interity(bool mustBeValid, char *board);
bool is_board_valid(char *board);
bool is_solved(char *board);
bool solve(int i, int j, char *board);
