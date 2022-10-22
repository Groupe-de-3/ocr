#include "gaussian_blur.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "kernel_convolution.h"
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

void gaussian_blur_run(ImageView *in, ImageView *out, float std_deriv) {
    size_t kernel_size = gaussian_blur_optimal_kernel_size(std_deriv);
    float *kernel      = m_new(float, kernel_size, kernel_size);
    gaussian_blur_populate_kernel(kernel, std_deriv);

    filter_kernel_run(in, out, kernel);

    m_destroy(kernel);
}
