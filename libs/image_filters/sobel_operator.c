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

    float max_gradient = 0.;
    for (int y = 0; y < in->height; y++) {
        for (int x = 0; x < in->width; x++) {
            float x_gradient =
                img_some_to_grayscale(filter_kernel_run_at(in, x_kernel, x, y));
            float y_gradient =
                img_some_to_grayscale(filter_kernel_run_at(in, y_kernel, x, y));

            if (gradient_dir_out != NULL) {
                float gradient_dir = atan2f(x_gradient, y_gradient);
                imgv_set_pixel_grayscale(gradient_dir_out, x, y, gradient_dir);
            }
            if (gradient_out != NULL) {
                float gradient_val =
                    sqrtf(x_gradient * x_gradient + y_gradient * y_gradient);
                imgv_set_pixel_grayscale(gradient_out, x, y, gradient_val);
                if (gradient_val > max_gradient)
                    max_gradient = gradient_val;
            }
        }
    }
    if (gradient_out != NULL) {
        for (int y = 0; y < in->height; y++) {
            for (int x = 0; x < in->width; x++) {
                imgv_set_pixel_grayscale(
                    gradient_out, x, y,
                    imgv_get_pixel_grayscale(gradient_out, x, y) / max_gradient
                );
            }
        }
    }

    m_destroy(x_kernel);
    m_destroy(y_kernel);
}
