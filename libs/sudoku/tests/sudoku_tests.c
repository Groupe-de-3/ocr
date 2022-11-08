#include "tests.h"

#include "sudoku.h"

const char *test_functions[] = {
    "is_valid_test1",
     NULL
};

void is_valid_test1(void) {
    char invalid_sudoku1[] = {
    
    };
    
    h_assert(is_board_valid(invalid_sudoku1) == false);
}
