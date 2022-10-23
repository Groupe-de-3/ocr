#pragma once

#include "image_view.h"

typedef struct {
    float distance;
    float angle;
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

HoughLine *hough_extract_lines(ImageView *space);
