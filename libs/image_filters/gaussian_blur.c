#include "gaussian_blur.h"

#define _GNU_SOURCE
#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "matrices.h"

size_t gaussian_blur_optimal_kernel_size(float param) {
    size_t size = (size_t)ceil(6.f * param);
    return size + (1 - size & 0x1); // Make sure we return an odd value.
}
void populate_gaussian_blur_kernel(float *kernel_mat, float param) {
    float factor = 1.f / (2.f * M_PIf * param * param);

    size_t w     = m_width(kernel_mat);
    size_t haf_w = w / 2;
    size_t h     = m_height(kernel_mat);
    size_t haf_h = h / 2;
    assert((w & 0x1) == 1 && (h & 0x1) == 1);

    for (size_t y = 0; y < h; y++) {
        float dy = (float)y - (float)haf_h;
        for (size_t x = 0; x < w; x++) {
            float dx = (float)x - (float)haf_w;
            float value =
                factor * expf(-(dx * dx + dy * dy) / (2.f * param * param));
            m_get2(kernel_mat, x, y) = value;
        }
    }
}
