#include <assert.h>
#include <err.h>
#include <stdio.h>

#include "gaussian_blur.h"
#include "images.h"
#include "img_formats/bmp.h"
#include "kernel_convolution.h"
#include "matrices.h"
#include "sobel_operator.h"

void print_m(float *m) {
    for (size_t y = 0; y < m_height(m); y++) {
        for (size_t x = 0; x < m_width(m); x++) {
            printf("%03.4f", m_get2(m, x, y));
            printf(" - ");
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        errx(
            1,
            "Expected at least one argument\n   Usage: %s <input_image_path>",
            argv[0]
        );
    }

    printf("Reading file %s\n", argv[1]);
    Image           image;
    ImageLoadResult rslt =
        img_load_file(argv[1], &image, false, PixelFormat_Rgbf);
    if (!rslt.success) {
        errx(1, "Could not load");
    }
    ImageView image_view    = imgv_default(&image);
    image_view.wraping_mode = WrappingMode_Clamp;

    // Save input
    bmp_save_to_path("in.bmp", &image);

    // Bluring image
    Image blured = img_new(image.width, image.height, image.format);
    imgv_default(&blured);

    ImageView blur_view    = imgv_default(&blured);
    blur_view.wraping_mode = WrappingMode_Clamp;

    // Executing blur
    printf("Bluring image\n");
    gaussian_blur_run(&image_view, &blur_view, 2.f);

    // Saving blur
    bmp_save_to_path("blured.bmp", &blured);

    printf("Computing gradient\n");
    // Computing image's gradient
    Image     gradient      = img_new(image.width, image.height, image.format);
    ImageView gradient_view = imgv_default(&gradient);

    sobel_execute(&blur_view, &gradient_view, NULL);
    bmp_save_to_path("gradient.bmp", &gradient);

    img_destroy(image);
    img_destroy(blured);
    img_destroy(gradient);
    return 0;
}
