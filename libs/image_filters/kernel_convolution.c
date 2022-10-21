#include "kernel_convolution.h"

#include <assert.h>
#include <stdio.h>

#include "image_view.h"
#include "matrices.h"

some_pixel_t filter_kernel_run_at(ImageView *src, float *kernel, int x, int y) {
    assert(m_dimc(kernel) == 2);
    assert((m_dimv(kernel)[0] & 0x1) == 1); // Kernel must have odd dimensions
    assert((m_dimv(kernel)[1] & 0x1) == 1);

    // RGB8 have a difference behavior
    if (src->image->format == PixelFormat_Rgb8 || src->image->format == PixelFormat_Rgbf) {
        int half_size_x = (int)m_dimv(kernel)[0] / 2;
        int half_size_y = (int)m_dimv(kernel)[1] / 2;

        float acc[3] = {0., 0., 0.};
        for (int dx = -half_size_x; dx <= half_size_x; dx++) {
            int kx = dx + half_size_x;
            for (int dy = -half_size_y; dy <= half_size_y; dy++) {
                int ky = dy + half_size_y;

                rgbf_pixel_t pixel_value =
                    imgv_get_pixel_rgbf(src, x + dx, y + dy);
                float kernel_weight = m_get2(kernel, (size_t)kx, (size_t)ky);
                acc[0] += pixel_value.r * kernel_weight;
                acc[1] += pixel_value.g * kernel_weight;
                acc[2] += pixel_value.b * kernel_weight;
            }
        }

        return (some_pixel_t
        ){.format = PixelFormat_Rgbf,
          .value  = {
               .rgbf = {
                   .r = acc[0],
                   .g = acc[1],
                   .b = acc[2]}}};
    }
    else {
        int half_size_x = (int)m_dimv(kernel)[0] / 2;
        int half_size_y = (int)m_dimv(kernel)[1] / 2;

        grayscale_pixel_t total_value = 0.;
        for (int dx = -half_size_x; dx <= half_size_x; dx++) {
            int kx = dx + half_size_x;
            for (int dy = -half_size_y; dy <= half_size_y; dy++) {
                int ky = dy + half_size_y;

                grayscale_pixel_t pixel_value =
                    imgv_get_pixel_grayscale(src, x + dx, y + dy);
                float kernel_weight = m_get2(kernel, (size_t)kx, (size_t)ky);
                total_value +=
                    (grayscale_pixel_t)((float)pixel_value * kernel_weight);
            }
        }
        return (some_pixel_t
        ){.format = PixelFormat_GrayScale, .value = {.grayscale = total_value}};
    }
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
