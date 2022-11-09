#pragma once

#include "image_view.h"
#include "matrices.h"
#include "shapes.h"

some_pixel_t bilinear_sample(ImageView *img, float x, float y);

void bilinear_resize(ImageView *from, ImageView *to);

void bilinear_apply_matrix(ImageView *from, ImageView *to, Matrix(double) m);
void bilinear_perspective_transmute(
    ImageView *from_img, fquadrilateral_t from_shape, ImageView *to,
    fquadrilateral_t to_shape
);
