#include "kernel_convolution.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "image_view.h"
#include "matrices.h"

some_pixel_t filter_kernel_run_at(ImageView *src, float *kernel, int x, int y) {
    assert(m_dimc(kernel) == 2);
    assert((m_dimv(kernel)[0] & 0x1) == 1); // Kernel must have odd dimensions
    assert((m_dimv(kernel)[1] & 0x1) == 1);

    // RGB8 have a difference behavior
    int half_size_x = (int)m_dimv(kernel)[0] / 2;
    int half_size_y = (int)m_dimv(kernel)[1] / 2;

    enum PixelFormat target_format =
        img_equivalent_float_channels_format(src->image->format);
    size_t channel_length = img_float_channels_length(target_format);

    // channel_length won't ever go above 4 or 5 so no stack problem.
    // The accumulator for the channel's values
    float acc[channel_length];
    memset(acc, 0, sizeof(float) * channel_length);

    for (int dx = -half_size_x; dx <= half_size_x; dx++) {
        int kx = dx + half_size_x;
        for (int dy = -half_size_y; dy <= half_size_y; dy++) {
            int ky = dy + half_size_y;

            any_pixel_t pixel_value = img_some_to_any(
                imgv_get_pixel_some(src, x + dx, y + dy), target_format
            );
            float kernel_weight = m_get2(kernel, (size_t)kx, (size_t)ky);
            for (size_t c = 0; c < channel_length; c++)
                acc[c] += pixel_value.float_channels[c] * kernel_weight;
        }
    }

    some_pixel_t output = {.format = target_format};
    for (size_t c = 0; c < channel_length; c++)
        output.value.float_channels[c] = acc[c];
    return output;
}

void filter_kernel_run(ImageView *src, ImageView *out, float *kernel) {
    assert(src->width == out->width);
    assert(src->height == out->height);

    assert(m_dimc(kernel) == 2);
    assert((m_dimv(kernel)[0] & 0x1) == 1); // The kernel sizes must be odd
    assert((m_dimv(kernel)[1] & 0x1) == 1);

    for (int y = 0; y < src->height; y++) {
        for (int x = 0; x < src->width; x++) {
            imgv_set_pixel_some(
                out, x, y, filter_kernel_run_at(src, kernel, x, y)
            );
        }
    }
}
