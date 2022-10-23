#pragma once

#include "image_view.h"

typedef struct {
    float high_threshold;
    float low_threshold;
    
    float extend_angle_limit;
    float output_value_for_soft_edges;
} CannyParameters;

void canny_run(ImageView *gradient, ImageView *gradient_direction, ImageView *out, CannyParameters parameters);

CannyParameters canny_guess_parameters(ImageView *gradient);
