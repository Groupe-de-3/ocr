//
// Created by romai on 05/10/20220
//

#include "stdio.h"
#include "sudoku.h"

int main() {
    char sudoku[] = {4, 0, 0, 0, 0, 0, 8, 0, 5, 0, 3, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0,
                     6, 0, 0, 0, 0, 0, 8, 0, 4, 0, 0, 0, 0, 0, 0, 1, 0,
                     0, 0, 0, 0, 0, 0, 6, 0, 3, 0, 7, 0, 5, 0, 0, 2, 0,
                     0, 0, 0, 0, 1, 0, 4, 0, 0, 0, 0, 0, 0};
    //printf("%i \n" , check_lines(sudoku));
    //printf("%i \n" , check_columns(sudoku));
    //printf("%i \n" , check_squares(sudoku,0,0));
    //printf("%i \n" , is_solved(sudoku));
    //printf("%i \n" ,  __sudoku_solve(sudoku,0,0));
    printf("%i \n", solve(0, 0, sudoku));
    sudoku_print(sudoku);
}
