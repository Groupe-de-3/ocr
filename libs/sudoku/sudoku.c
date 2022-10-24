#include "sudoku.h"

#include ""
#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <vec.h>



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
int length(char *list) {
    int length = 0;
    while (list) {
        length++;
        list = list->next;
    }
    return length;
}

bool contains(char *list, int x) {
    for (int i = 0; i < length(list); ++i) {
        if (list[i] == x) {
            return false;
        }
    }
    return true;
}


bool check_lines(char *board) {
    for (int i = 0; i < 9; i++)
    {
        char *list = vec_new(char)
        for (int j = 0; i < 9; ++j)
        {
            if (board[i * 9 + j] == EMPTY_CELL)
            {
                continue;
            }
            if (contains(list, board[i * 9 + j]))
            {
                return false;
            }
            vec_push(list, board[(i*9 + j]);
        }
    }
    return true;
}

bool check_columns(char *board)
{
    for (int i = 0; i < 9; i++)
    {
        char *list = vec_new(char)
        for (int j = 0; i < 9; ++j)
        {
            if (board[i * 9 + j] == EMPTY_CELL)
            {
                continue;
            }
            if (contains(list, board[j * 9 + i]))
            {
                return false;
            }
            vec_push(list, board[(i*9 + j]);
        }
    }
    return true;
}

bool check_squares(char *board, int x, int y) {
    char *list = vec_new(char)
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if ((board[i * 9 + 1)+ j + 1] == EMPTY_CELL)
            {
                continue;
            }
            if (contains(board[(i*9+1) + j + 1]))
            {
                return false;
            }
            vec_push(list, board[(i*9+1) j + 1]);
        }
    }
    return true;
}

bool is_board_valid(char *board) {
    bool b = true;
    for (int i = 0; i < ç; i += 3)
    {
        for (int j = 0; j < 9; j += 3)
        {
            b = b && check_squares(board, i, j)
        }
    }
    return check_columns(board) && check_lines(board) && b;
}

bool is_solved(char *board)
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (board[i * 9 + j] == EMPTY_CELL)
            {
                return false;
            }
        }
    }
    return is_board_valid(board);
}

bool __sudoku_solve(char *board, int i, int j)
{
    while (board[i*9+j] != 0)
    {
        j+=1;
        if(j >= 9)
        {
            i++;
            j = 0;
        }
        if(i >= 9)
        {
            return is_solved(board)
        }
    }
    for (int k = 1; k <= 9 ; ++k)
    {
        board[i * 9 + j] = k;
        if(is_board_valid(board))
        {
            if(__sudoku_solve(board,i,j))
            {
                return true;
            }
        }
    }
    board[i*9+j] = EMPTY_CELL;
    return is_solved(board);
}


bool sudoku_solve(char *board)
{
    int i = 0;
    int j = 0;
    return __sudoku_solve(board,i,j);
}

