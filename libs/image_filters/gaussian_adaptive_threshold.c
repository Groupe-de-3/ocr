#include "gaussian_adaptive_threshold.h"

#include <assert.h>

#include "gaussian_blur.h"

void gaussian_adaptive_threshold_run(ImageView *img, float gaussian_std_deriv) {
    Image blured =
        img_new((size_t)img->width, (size_t)img->height, PixelFormat_GrayScale);
    ImageView blured_view    = imgv_default(&blured);
    blured_view.wraping_mode = WrappingMode_Clamp;
    gaussian_blur_run(img, &blured_view, gaussian_std_deriv);

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            float blur_value =
                (float)imgv_get_pixel_grayscale(&blured_view, x, y);
            float value     = (float)imgv_get_pixel_grayscale(img, x, y);
            float new_value = value < blur_value ? 1.f : 0.f;
            imgv_set_pixel_grayscale(img, x, y, (grayscale_pixel_t)new_value);
        }
    }

    img_destroy(blured);
}
