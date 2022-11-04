#include "shapes.h"

#include <assert.h>
#include <stdio.h>

iquadrilateral_t f2iquadrilateral(fquadrilateral_t from) {
    return (iquadrilateral_t){
        .a = f2ipoint2d(from.a),
        .b = f2ipoint2d(from.b),
        .c = f2ipoint2d(from.c),
        .d = f2ipoint2d(from.d),
    };
}
fquadrilateral_t i2fquadrilateral(iquadrilateral_t from) {
    return (fquadrilateral_t){
        .a = i2fpoint2d(from.a),
        .b = i2fpoint2d(from.b),
        .c = i2fpoint2d(from.c),
        .d = i2fpoint2d(from.d),
    };
}

void la_subdivide_concave_iquadrilateral(
    iquadrilateral_t from, int width, int height, iquadrilateral_t *out
) {
    assert(width >= 0);
    assert(height >= 0);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            ipoint2d_t up_a = ipoint2d_interpolate(from.a, from.b, x, width);
            ipoint2d_t up_b =
                ipoint2d_interpolate(from.a, from.b, x + 1, width);
            ipoint2d_t down_a = ipoint2d_interpolate(from.d, from.c, x, width);
            ipoint2d_t down_b =
                ipoint2d_interpolate(from.d, from.c, x + 1, width);

            ipoint2d_t a = ipoint2d_interpolate(up_a, down_a, y, height);
            ipoint2d_t b = ipoint2d_interpolate(up_b, down_b, y, height);
            ipoint2d_t c = ipoint2d_interpolate(up_b, down_b, y + 1, height);
            ipoint2d_t d = ipoint2d_interpolate(up_a, down_a, y + 1, height);

            out[y * width + x] = (iquadrilateral_t){
                .a = a,
                .b = b,
                .c = c,
                .d = d,
            };
        }
    }
}

void la_perspective_subdivide_concave_fquadrilateral(
    fquadrilateral_t from, int width, int height, fquadrilateral_t *out
) {
    assert(width >= 0);
    assert(height >= 0);

    float top_width    = from.b.x - from.a.x;
    float bottom_width = from.c.x - from.d.x;
    float z            = top_width / bottom_width;

    for (int y = 0; y < height; y++) {
        float dy = (float)y / (float)height;
        dy       = (z - 1.f) * dy * dy * dy + (1.f - z) * dy * dy + dy;

        for (int x = 0; x < width; x++) {
            float dx = (float)x / (float)width;

            fpoint2d_t up_a   = fpoint2d_interpolate(from.a, from.b, dx);
            fpoint2d_t up_b   = fpoint2d_interpolate(from.a, from.b, dx);
            fpoint2d_t down_a = fpoint2d_interpolate(from.d, from.c, dx);
            fpoint2d_t down_b = fpoint2d_interpolate(from.d, from.c, dx);

            fpoint2d_t a = fpoint2d_interpolate(up_a, down_a, dy);
            fpoint2d_t b = fpoint2d_interpolate(up_b, down_b, dy);
            fpoint2d_t c = fpoint2d_interpolate(up_b, down_b, dy);
            fpoint2d_t d = fpoint2d_interpolate(up_a, down_a, dy);

            out[y * width + x] = (fquadrilateral_t){
                .a = a,
                .b = b,
                .c = c,
                .d = d,
            };
        }
    }
}
