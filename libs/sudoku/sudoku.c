#include "sudoku.h"
#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <vec.h>



void sudoku_print(char *board)
{
    for (int i = 0; i < 9; ++i)
    {
        printf("------------------------------------\n");
        for (int j = 0; j < 9; ++j) {
            if (board[i * 9 + j] != 0)
            {
                printf("% 3i|", board[i * 9 + j]);
            }
            else
            {
                printf("   |");
            }
        }
        printf("\n");
    }
    printf("------------------------------------\n");
}

bool contains(char *list, int x)
{
    for (size_t i = 0; i < vec_size(list); ++i)
    {
        if (list[i] == x) {
            return true;
        }
    }
    return false;
}


bool check_lines(char *board)
{
    for (int i = 0; i < 9; i++)
    {
        char *list = vec_new(char);
        for (int j = 0; j < 9; ++j)
        {
            if (board[i * 9 + j] == EMPTY_CELL)
            {
                continue;
            }
            if (contains(list, board[i * 9 + j]))
            {
                vec_destroy(list);
                return false;
            }
            *vec_push(&list, char) =  board[i * 9 + j];
        }
        vec_destroy(list);
    }
    return true;
}

bool check_columns(char *board)
{
    for (int i = 0; i < 9; i++)
    {
        char *list = vec_new(char);
        for (int j = 0; j < 9; ++j)
        {
            if (board[j * 9 + i] == EMPTY_CELL)
            {
                continue;
            }
            if (contains(list, board[j * 9 + i]))
            {
                vec_destroy(list);
                return false;
            }
            *vec_push(&list, char) =  board[j * 9 + i];
        }
        vec_destroy(list);
    }
    return true;
}

bool check_squares(char *board, int x, int y)
{
    char *list = vec_new(char);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (board[(i + x)* 9 + j + y] == EMPTY_CELL)
            {
                continue;
            }
            if (contains(list, board[(i + x)*9 + j + y]))
            {
                vec_destroy(list);
                return false;
            }
            *vec_push(&list, char) =  board[(i+x)*9 + j + y];
        }
    }
    vec_destroy(list);
    return true;
}

bool is_board_valid(char *board)
{
    bool b = true;
    for (int i = 0; i < 9; i += 3)
    {
        for (int j = 0; j < 9; j += 3)
        {
            b = b && check_squares(board, i, j);
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
    while (board[i*9+j] != EMPTY_CELL)
    {
        j+=1;
        if(j >= 9)
        {
            i++;
            j = 0;
        }
        if(i >= 9)
        {
            return is_solved(board);
        }
    }
    for (char k = 1; k <= 9 ; ++k)
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


