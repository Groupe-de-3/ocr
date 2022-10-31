#pragma once

typedef struct {
    float x, y;
} fpoint2d_t;

typedef struct {
    int x, y;
} ipoint2d_t;

fpoint2d_t fpoint2d(float x, float y);
ipoint2d_t ipoint2d(int x, int y);

ipoint2d_t f2ipoint2d(fpoint2d_t p);
fpoint2d_t i2fpoint2d(ipoint2d_t p);

fpoint2d_t fpoint2d_interpolate(fpoint2d_t from, fpoint2d_t to, float d);
ipoint2d_t ipoint2d_interpolate(ipoint2d_t from, ipoint2d_t to, int a, int b);
