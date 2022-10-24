#pragma once

#include "image_view.h"

typedef struct {
    float r;
    float theta;
} HoughLine;

void hough_acc_space_run_at(
    ImageView *in, ImageView *gradient_dir, ImageView *out, int x, int y
);
void hough_acc_space_run(
    ImageView *in, ImageView *gradient_dir, ImageView *out
);
void hough_acc_space_probabilistic_run(
    ImageView *in, ImageView *gradient_dir, ImageView *out, size_t m
);

void hough_acc_space_draw_lines(ImageView *acc_space, ImageView *out);

HoughLine *hough_extract_lines(ImageView *space);
