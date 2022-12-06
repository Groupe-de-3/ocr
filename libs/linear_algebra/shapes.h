#pragma once

#include "point2d.h"

typedef struct {
    ipoint2d_t a;
    ipoint2d_t b;
    ipoint2d_t c;
    ipoint2d_t d;
} iquadrilateral_t;

typedef struct {
    fpoint2d_t a;
    fpoint2d_t b;
    fpoint2d_t c;
    fpoint2d_t d;
} fquadrilateral_t;

iquadrilateral_t f2iquadrilateral(fquadrilateral_t from);
fquadrilateral_t i2fquadrilateral(iquadrilateral_t from);

void la_subdivide_concave_iquadrilateral(
    iquadrilateral_t from, int width, int height, iquadrilateral_t *out
);

void la_perspective_subdivide_concave_fquadrilateral(
    fquadrilateral_t from, int width, int height, fquadrilateral_t *out
);

typedef struct {
    ipoint2d_t position;
    ipoint2d_t size;
} irectangle_t;
