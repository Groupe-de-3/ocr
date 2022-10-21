#include "sobel_operator.h"

#include <assert.h>
#include <math.h>

#include "kernel_convolution.h"
#include "matrices.h"
#include "utils_countargs.h"

void sobel_execute(
    ImageView *in, ImageView *gradient_out, ImageView *gradient_dir_out
) {
    assert(in != NULL);
    assert(
        gradient_out == NULL ||
        (in->width == gradient_out->width && in->height == gradient_out->height)
    );
    assert(
        gradient_dir_out == NULL || (in->width == gradient_dir_out->width &&
                                     in->height == gradient_dir_out->height)
    );

    // If both images are NULL, there is nothing to output
    if (gradient_out == NULL && gradient_dir_out == NULL)
        return;

    float *x_kernel = m_new(float, 3, 3);
    m_copy(
        x_kernel,
        (float[]){
            1,
            0,
            -1,
            2,
            0,
            -2,
            1,
            0,
            -1,
        }
    );

    float *y_kernel = m_new(float, 3, 3);
    m_copy(
        y_kernel,
        (float[]){
            1,
            2,
            1,
            0,
            0,
            0,
            -1,
            -2,
            -1,
        }
    );

    float max_gradient[3] = { 0.f, 0.f, 0.f };
    for (int y = 0; y < in->height; y++) {
        for (int x = 0; x < in->width; x++) {
            rgbf_pixel_t x_gradient =
                img_some_to_rgbf(filter_kernel_run_at(in, x_kernel, x, y));
            rgbf_pixel_t y_gradient =
                img_some_to_rgbf(filter_kernel_run_at(in, y_kernel, x, y));

            if (gradient_dir_out != NULL) {
                rgbf_pixel_t gradient_dir = {
                    .r = atan2f(x_gradient.r, y_gradient.r),
                    .g = atan2f(x_gradient.b, y_gradient.g),
                    .b = atan2f(x_gradient.b, y_gradient.b),
                };
                imgv_set_pixel_rgbf(gradient_dir_out, x, y, gradient_dir);
            }
            if (gradient_out != NULL) {
                rgbf_pixel_t gradient_val = {
                    .r = sqrtf(x_gradient.r*x_gradient.r + y_gradient.r*y_gradient.r),
                    .g = sqrtf(x_gradient.g*x_gradient.g + y_gradient.g*y_gradient.g),
                    .b = sqrtf(x_gradient.b*x_gradient.b + y_gradient.b*y_gradient.b),
                };
                imgv_set_pixel_rgbf(gradient_out, x, y, gradient_val);
                if (gradient_val.r > max_gradient[0])
                    max_gradient[0] = gradient_val.r;
                if (gradient_val.g > max_gradient[1])
                    max_gradient[1] = gradient_val.g;
                if (gradient_val.b > max_gradient[2])
                    max_gradient[2] = gradient_val.b;
            }
        }
    }
    if (gradient_out != NULL) {
        for (int y = 0; y < in->height; y++) {
            for (int x = 0; x < in->width; x++) {
                rgbf_pixel_t value = imgv_get_pixel_rgbf(gradient_out, x, y);
                imgv_set_pixel_rgbf(
                    gradient_out, x, y,
                    (rgbf_pixel_t) {
                        value.r / max_gradient[0],
                        value.g / max_gradient[1],
                        value.b / max_gradient[2],
                    }
                );
            }
        }
    }

    m_destroy(x_kernel);
    m_destroy(y_kernel);
}
