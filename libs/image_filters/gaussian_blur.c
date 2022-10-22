#include "gaussian_blur.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "kernel_convolution.h"
#include "box_blur.h"
#include "matrices.h"

static const float PI = (float)M_PI;

static size_t gaussian_blur_optimal_kernel_size(float param) {
    size_t size = (size_t)ceil(6.f * param);
    // Make sure we return an odd value.
    return size + (1 - size & 0x1);
}

static void gaussian_blur_populate_kernel(float *kernel_mat, float param) {
    float factor = 1.f / (2.f * PI * param * param);

    size_t w     = m_width(kernel_mat);
    size_t haf_w = w / 2;
    size_t h     = m_height(kernel_mat);
    size_t haf_h = h / 2;

    // Make sure the dimensions are odd
    assert((w & 0x1) == 1 && (h & 0x1) == 1);

    float sum = 0.;
    for (size_t y = 0; y < h; y++) {
        float dy = (float)y - (float)haf_h;
        for (size_t x = 0; x < w; x++) {
            float dx = (float)x - (float)haf_w;
            float value =
                factor * expf(-(dx * dx + dy * dy) / (2.f * param * param));
            m_get2(kernel_mat, x, y) = value;
            sum += value;
        }
    }

    // Normalize the output
    for (size_t y = 0; y < h; y++)
        for (size_t x = 0; x < w; x++)
            m_get2(kernel_mat, x, y) /= sum;
}

// Sadly it's really too much math to fully understand by myself
// So this is a copy paste, from the copy paste of
// https://blog.ivank.net/fastest-gaussian-blur.html
static void boxes(float sigma, size_t n, size_t *sizes) {
    float wIdeal = sqrtf((12*sigma*sigma/(float)n)+1);
    size_t wl = (size_t) floorf(wIdeal); 
    if (wl % 2 == 0)
        wl--;
    size_t wu = wl + 2;
    float mIdeal = (12*sigma*sigma - (float)(n*wl*wl) - (float)(4*n*wl) - (float)(3*n))/(-4*(float)wl - 4);
    size_t m = (size_t)roundf(mIdeal);

    for (size_t i = 0; i < n; i++)
        sizes[i] = i < m ? wl : wu;
}

void gaussian_blur_run(ImageView *in, ImageView *out, float std_deriv) {
    size_t box_count = 3;

    size_t box_sizes[box_count];
    boxes(std_deriv, box_count, box_sizes);
    
    Image buffer_img = img_new((size_t)abs(in->width), (size_t)abs(in->height), in->image->format);
    ImageView buffer_view = imgv_default(&buffer_img);
    buffer_view.wraping_mode = WrappingMode_Clamp;
    
    ImageView *current_source = in;
    ImageView *current_target = out;
    for (size_t i = 0; i < box_count; i++)
        box_blur_run(current_source, current_target, box_sizes[i]);
    
    img_destroy(buffer_img);
}
