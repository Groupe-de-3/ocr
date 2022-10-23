#pragma once

#include "image_view.h"

some_pixel_t bilinear_sample(ImageView *img, float x, float y);

void bilinear_resize(ImageView *from, ImageView *to);
