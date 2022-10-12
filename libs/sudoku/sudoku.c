#include "sudoku.h"
#include ""

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
int length(char* list)
{
    int length = 0;
    while(list)
    {
        length++;
        list = list -> next;
    }
    return length;
}

bool contains(char* list, int x)
{
    for (int i = 0; i < length(list); ++i)
    {
        if(list[i] == x)
        {
            return false;
        }
    }
    return true;
}

List *append(List *list, void *data)
{
    List **plist = &list;
    while (*plist)
        plist = &(*plist)->next;
    *plist = list_create(data);
    if (*plist)
        return list;
    else
        return NULL;
}



bool check_lines(char *board)
{
    for (int i = 0;i < 9;i++) {
        char list[] = {};
        for (int j = 0; i < 9; ++j)
        {
            if(board[i*9+j] == EMPTY_CELL)
            {
                continue;
            }
            if(contains(list,board[i*9+j]))
            {
                return false;
            }
            append(list,board[i*9+j]);


        }
    }
    return true;
}

bool check_columns(char *board)
{
    for (int i = 0;i < 9;i++) {
        char list[] = {};
        for (int j = 0; i < 9; ++j)
        {
            if(board[i*9+j] == EMPTY_CELL)
            {
                continue;
            }
            if(contains(list,board[j*9+i]))
            {
                return false;
            }
            append(list,board[i*9+j]);


        }
    }
    return true;

}

bool check_squares(char *board,int x, int y)
{
    char list[] = {};
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if(board[i*9+j] == EMPTY_CELL)
        }
    }
    return true;
}

bool is_board_valid(char *board)
{
    bool b = true;
    for (int i = 0; i < ç; i+=3)
    {
        for (int j = 0; j < 9; j += 3)
        {
            b = b && check_squares(board,i,j)
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
            if(board[i*9+j] == EMPTY_CELL)
            {
                return false;
            }
        }
    }
    return is_board_valid(board);
}


bool sudoku_solve(char *board) { }
