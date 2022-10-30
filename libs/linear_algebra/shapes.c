#include "shapes.h"

#include <assert.h>

void la_subdivide_concave_iquadrilateral(iquadrilateral_t from, int width, int height, iquadrilateral_t *out) {
    assert(width >= 0);
    assert(height >= 0);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            ipoint2d_t up_a   = ipoint2d_interpolate(from.a, from.b, x    , width);
            ipoint2d_t up_b   = ipoint2d_interpolate(from.a, from.b, x + 1, width);
            ipoint2d_t down_a = ipoint2d_interpolate(from.d, from.c, x    , width);
            ipoint2d_t down_b = ipoint2d_interpolate(from.d, from.c, x + 1, width);
            
            ipoint2d_t a = ipoint2d_interpolate(up_a, down_a, y    , height);
            ipoint2d_t b = ipoint2d_interpolate(up_b, down_b, y    , height);
            ipoint2d_t c = ipoint2d_interpolate(up_b, down_b, y + 1, height);
            ipoint2d_t d = ipoint2d_interpolate(up_a, down_a, y + 1, height);
            
            out[y * width + x] = (iquadrilateral_t){
                .a = a, .b = b, .c = c, .d = d,
            };
        }
    }
}
