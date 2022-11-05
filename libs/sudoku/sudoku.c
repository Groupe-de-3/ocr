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


/*bool check_lines(char *board)
{
    for (int i = 0; i < 9; i++)
    {
        char *list = vec_new(char);
        for (int j = 0; j < 9; ++j)
        {
            if (board[i * 9 + j] != EMPTY_CELL)
            {
                if (contains(list, board[i * 9 + j]))
                    return false;
                *vec_push(&list, char) =  board[i * 9 + j];

            }
        }
    }
    return true;
}

bool check_columns(char *board) {
    for (int i = 0; i < 9; i++)
    {
        char *list = vec_new(char);
        for (int j = 0; j < 9; j++)
        {
            if (board[j * 9 + i] != EMPTY_CELL)
            {
                if (contains(list, board[j * 9 + i]))
                    return false;
                *vec_push(&list, char) = board[j * 9 + i];
            }
        }
    }
    return true;
}



bool check_squares(char *board)
{
    for (int i = 0; i < 9; i += 3)
    {
        for(int j = 0; j < 9; j += 3)
        {
            char *list = vec_new(char);
            for(int x = 0; x < 3; x++)
            {
                for(int y = 0; y < 3; y++)
                {
                    if (board[x * 9 + y] != EMPTY_CELL)
                    {
                        if (contains(list, board[x * 9 + y]))
                            return false;
                        *vec_push(&list, char) =  board[x * 9 + y];
                    }
                }
            }

        }
    }

    return true;
}

bool is_board_valid(char *board)
{
    return check_squares(board) && check_columns(board) && check_lines(board);
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
            if (check_squares(board) && check_lines(board) && check_columns(board))
                continue;
            return false;
        }
    }
    return true;
}


bool sudoku_solve(char *board)
{
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            if (board[i * 9 + j] == EMPTY_CELL)
            {
                for (char num = 1; num < 10; num++)
                {
                    board[i * 9 + j] = num;
                    if (is_board_valid(board))
                        if(sudoku_solve(board))
                            return true;
                    board[i * 9 + j] = EMPTY_CELL;
                }
                if (board[i * 9 + j] == EMPTY_CELL)
                    return false;
            }
        }
    }
    return is_solved(board);
}

*/

bool checkBoardInterity (bool mustBeValid, char* board)
{
    for (int blockI = 0; blockI < 3; ++blockI) {
        for (int blockJ = 0; blockJ < 3; ++blockJ)
        {
            for (int i = 0; i < 3; ++i)
            {unsigned int foundBitMap = 0;
                for (int j = 0; j < 3; ++j)
                {
                    unsigned int bit =
                        1u << board[(blockI + i) * 3 + (blockJ + j) * 3];
                    if ((foundBitMap & bit) != 0 && bit != 1)
                        return false;
                    foundBitMap |= bit;
                }
                if (mustBeValid && foundBitMap != 0x3FE)
                    return false;
            }
        }}

    for (int i = 0; i < 9; i++)
    {
        unsigned int foundBitMap = 0;
        for (int j = 0; j < 9; j++)
        {
            unsigned int bit = 1u << board[i * 9 + j];
            if ((foundBitMap & bit) != 0 && bit != 1)
                return false;
            foundBitMap |= bit;
        }
        if (mustBeValid && foundBitMap != 0x3FE)
            return false;
    }
    for (int j = 0; j < 9; j++)
    {
        unsigned int foundBitMap = 0;
        for (int i = 0; i < 9; i++)
        {
            unsigned int bit = 1u << board[j * 9 + i];
            if ((foundBitMap & bit) != 0 && bit != 1)
                return false;
            foundBitMap |= bit;
        }
        if (mustBeValid && foundBitMap != 0x3FE)
            return false;
    }

    return true;
}

bool IsBoardValid(char* board)
{
    return checkBoardInterity(false,board);
}

bool IsSolved(char *board)
{
    return checkBoardInterity(true,board);
}

bool Solve(int i, int j,char* board)
{
    while (j < 9 && board[i*9+j] != 0)
    {
        i++;
        if(i >= 9)
        {
            i = 0;
            j+=1;
        }
    }

    if(j >= 9)
    {
        return IsSolved(board);
    }
    for (char k = 1; k <= 9; ++k)
    {
        board[i*9+j] = k;
        if(!IsBoardValid(board))
        {
            continue ;
        }
        sudoku_print(board);
        if(Solve(i,j,board))
        {
            return true;
        }
    }
    board[i*9+j] = 0;
    return false;
}
