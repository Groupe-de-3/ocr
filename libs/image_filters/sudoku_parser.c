#include "sudoku_parser.h"

#include <stdio.h>
#include <math.h>

#include "vec.h"

ParsedSudokuResult sudoku_parse_from_lines(HoughLine *lines_vec, int w, int h) {
    assert(vec_size(lines_vec) == 4);

    sudoku_parse_pixel_pos_t points[4];

    float mw = (float)w / 2.f;
    float mh = (float)h / 2.f;

    // Each of the first two line will have collisions with the two last lines
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            HoughLine la = lines_vec[i];
            float da = la.r;
            float ca = cosf(la.theta);
            float sa = sinf(la.theta);
            HoughLine lb = lines_vec[2+j];
            float db = lb.r;
            float cb = cosf(lb.theta);
            float sb = sinf(lb.theta);

            float py = (sa*db - da*sb) / (sa*cb - ca*sb);
            float px = (ca*db - da*cb) / (ca*sb - sa*cb);
            px += mw;
            py += mh;
            
            points[i*2 + j] = (sudoku_parse_pixel_pos_t){ .x = (int)roundf(px), .y = (int)roundf(py) };
        }
    }
    
    return (ParsedSudokuResult) {
        .corners = { points[0], points[1], points[2], points[3] },
    };
}
