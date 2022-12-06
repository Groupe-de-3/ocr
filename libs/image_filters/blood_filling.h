#pragma once

#include "image_view.h"

typedef long (*blood_fill_weighter)(ImageView *imgv, int x, int y);

long blood_fill_center_weighter(ImageView *imgv, int x, int y);

void blood_fill_largest_weighted_blob(ImageView *img, blood_fill_weighter weighter);
void blood_fill_largest_blob(ImageView *img);
