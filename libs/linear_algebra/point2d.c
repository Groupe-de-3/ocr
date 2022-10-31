#include "point2d.h"

#include <math.h>

fpoint2d_t fpoint2d(float x, float y) {
    return (fpoint2d_t) {
        .x = x,
        .y = y,
    };
}

ipoint2d_t ipoint2d(int x, int y) {
    return (ipoint2d_t) {
        .x = x,
        .y = y,
    };
}

ipoint2d_t f2ipoint2d(fpoint2d_t p) {
    return ipoint2d((int)floor(p.x), (int)floor(p.y));
}
fpoint2d_t i2fpoint2d(ipoint2d_t p) {
    return fpoint2d((float)p.x, (float)p.y);
}

fpoint2d_t fpoint2d_interpolate(fpoint2d_t from, fpoint2d_t to, float d) {
    return fpoint2d(
        from.x * (1.f - d) + to.x * d,
        from.y * (1.f - d) + to.y * d
    );
}
ipoint2d_t ipoint2d_interpolate(ipoint2d_t from, ipoint2d_t to, int a, int b) {
    return ipoint2d(
        (from.x * (b - a)) / b + (to.x * a) / b,
        (from.y * (b - a)) / b + (to.y * a) / b
    );
}
