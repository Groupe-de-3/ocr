#pragma once

#include "hough_transform.h"
#include "image_view.h"
#include "shapes.h"
#include "point2d.h"

typedef struct {
    iquadrilateral_t shape;
    iquadrilateral_t cells[81];
} ParsedSudokuResult;

ParsedSudokuResult sudoku_parse_from_lines(HoughLine *lines_vec, int w, int h);
