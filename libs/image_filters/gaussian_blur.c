#include "gaussian_blur.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "box_blur.h"
#include "kernel_convolution.h"
#include "matrices.h"

// Sadly it's really too much math to fully understand by myself
// So this is a copy paste, from the copy paste of
// https://blog.ivank.net/fastest-gaussian-blur.html
static void boxes(float sigma, size_t n, size_t *sizes) {
    float  wIdeal = sqrtf((12 * sigma * sigma / (float)n) + 1);
    size_t wl     = (size_t)floorf(wIdeal);
    if (wl % 2 == 0)
        wl--;
    size_t wu     = wl + 2;
    float  mIdeal = (12 * sigma * sigma - (float)(n * wl * wl) -
                    (float)(4 * n * wl) - (float)(3 * n)) /
                   (-4 * (float)wl - 4);
    size_t m = (size_t)roundf(mIdeal);

    for (size_t i = 0; i < n; i++)
        sizes[i] = i < m ? wl : wu;
}

void gaussian_blur_run(ImageView *in, ImageView *out, float std_deriv) {
    size_t box_count = 3;

    size_t box_sizes[box_count];
    boxes(std_deriv, box_count, box_sizes);

    Image buffer_img = img_new(
        (size_t)abs(in->width), (size_t)abs(in->height), in->image->format
    );
    ImageView buffer_view    = imgv_default(&buffer_img);
    buffer_view.wraping_mode = WrappingMode_Clamp;

    ImageView *current_source = in;
    ImageView *current_target = (box_count & 0x1) == 1 ? out : &buffer_view;
    for (size_t i = 0; i < box_count; i++) {
        box_blur_run(current_source, current_target, box_sizes[i]);
        current_source = current_target;
        current_target = current_target == out ? &buffer_view : out;
    }

    assert(current_target == &buffer_view);

    img_destroy(buffer_img);
}
