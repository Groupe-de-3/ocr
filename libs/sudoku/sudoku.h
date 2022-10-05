#pragma once
#include <stdbool.h>
#define EMPTY_CELL -1

void sudoku_print(char *board);

bool sudoku_solve(char *board);
