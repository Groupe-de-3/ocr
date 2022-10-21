#include <assert.h>
#include <err.h>
#include <stdio.h>

#include "gaussian_blur.h"
#include "images.h"
#include "img_formats/bmp.h"
#include "kernel_convolution.h"
#include "matrices.h"

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
    assert(argc >= 2);

    Image           image;
    ImageLoadResult rslt = img_load_file(argv[1], &image);
    if (!rslt.success) {
        errx(1, "Could not load");
    }

    bmp_save_to_path("in.bmp", &image);

    Image blured = img_new(image.width, image.height, ImageFormat_GrayScale);
    imgv_default(&blured);

    float  target_blur = 5.f;
    size_t kernel_size = gaussian_blur_optimal_kernel_size(target_blur);

    float *blur_kernel = m_new(float, kernel_size, kernel_size);
    gaussian_blur_populate_kernel(blur_kernel, target_blur);
    print_m(blur_kernel);

    ImageView in_view    = imgv_default(&image);
    in_view.wraping_mode = WrappingMode_Repeat;
    ImageView out_view   = imgv_default(&blured);
    filter_kernel_run(&in_view, &out_view, blur_kernel);

    bmp_save_to_path("blured.bmp", &blured);

    img_destroy(image);
    img_destroy(blured);
    m_destroy(blur_kernel);
    return 0;
}
