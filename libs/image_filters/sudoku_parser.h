#pragma once

#include "hough_transform.h"
#include "image_view.h"

typedef struct {
    int sx, sy;
    int ex, ey;
} sudoku_cell_t;

typedef struct {
    int x, y;
} sudoku_parse_pixel_pos_t;

typedef struct {
    sudoku_parse_pixel_pos_t corners[4];
    sudoku_cell_t cells[9];
} ParsedSudokuResult;

ParsedSudokuResult sudoku_parse_from_lines(HoughLine *lines_vec, int w, int h);
