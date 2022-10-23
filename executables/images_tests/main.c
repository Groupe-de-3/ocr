#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#include "box_blur.h"
#include "gaussian_blur.h"
#include "images.h"
#include "img_formats/bmp.h"
#include "kernel_convolution.h"
#include "matrices.h"
#include "sobel_operator.h"
#include "canny_edge_detector.h"

static void print_m(float *m) {
    for (size_t y = 0; y < m_height(m); y++) {
        for (size_t x = 0; x < m_width(m); x++) {
            printf("%03.4f", m_get2(m, x, y));
            printf(" - ");
        }
        printf("\n");
    }
}

static long timediff(struct timeval start) {
    struct timeval now;
    gettimeofday(&now, NULL);
    return (now.tv_sec - start.tv_sec) * 1000 + now.tv_usec / 1000 -
           start.tv_usec / 1000;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        errx(
            1,
            "Expected at least one argument\n   Usage: %s <input_image_path>",
            argv[0]
        );
    }

    struct timeval start;

    gettimeofday(&start, NULL);
    printf("Reading file '%s'\n", argv[1]);
    Image           image;
    ImageLoadResult rslt =
        img_load_file(argv[1], &image, false, PixelFormat_Rgbf);
    if (!rslt.success) {
        errx(1, "Could not load");
    }
    ImageView image_view    = imgv_default(&image);
    image_view.wraping_mode = WrappingMode_Clamp;

    printf("    Loaded image of size %zu x %zu\n", image.width, image.height);

    printf("    Saving input to in.bmp\n");
    // Save input
    bmp_save_to_path("in.bmp", &image);
    printf("    Done (%ldms)\n", timediff(start));

    if (false) {
        // Bluring image using box blur
        Image box_blur = img_new(image.width, image.height, image.format);
        imgv_default(&box_blur);

        ImageView box_blur_view    = imgv_default(&box_blur);
        box_blur_view.wraping_mode = WrappingMode_Clamp;

        // Executing blur
        printf("Box Bluring image\n");
        box_blur_run(&image_view, &box_blur_view, 11);

        // Saving blur
        bmp_save_to_path("box_blured.bmp", &box_blur);

        img_destroy(box_blur);
    }

    gettimeofday(&start, NULL);
    printf("Bluring image\n");

    // Bluring image
    Image blured = img_new(image.width, image.height, image.format);
    imgv_default(&blured);

    ImageView blur_view    = imgv_default(&blured);
    blur_view.wraping_mode = WrappingMode_Clamp;

    // Executing blur
    gaussian_blur_run(&image_view, &blur_view, 2.f);

    printf("    Saving blur to blured.bmp\n");
    // Saving blur
    bmp_save_to_path("blured.bmp", &blured);
    printf("    Done (%ldms)\n", timediff(start));

    gettimeofday(&start, NULL);
    printf("Computing gradient\n");

    // Computing image's gradient
    Image     gradient      = img_new(image.width, image.height, image.format);
    ImageView gradient_view = imgv_default(&gradient);
    gradient_view.wraping_mode = WrappingMode_Clamp;
    Image     gradient_dir      = img_new(image.width, image.height, image.format);
    ImageView gradient_dir_view = imgv_default(&gradient_dir);
    gradient_dir_view.wraping_mode = WrappingMode_Clamp;

    sobel_execute(&blur_view, &gradient_view, &gradient_dir_view);
    printf("    Saving gradient to gradient.bmp\n");
    bmp_save_to_path("gradient.bmp", &gradient);
    printf("    Done (%ldms)\n", timediff(start));

    gettimeofday(&start, NULL);
    printf("Running Canny Edge Detector\n");
    // Computing image's gradient
    Image     edges      = img_new(image.width, image.height, PixelFormat_GrayScale);
    ImageView edges_view = imgv_default(&edges);

    canny_run(&gradient_view, &gradient_dir_view, &edges_view);
    printf("    Saving edges to edges.bmp\n");
    bmp_save_to_path("edges.bmp", &edges);
    printf("    Done (%ldms)\n", timediff(start));

    img_destroy(image);
    img_destroy(blured);
    img_destroy(gradient);
    img_destroy(gradient_dir);
    img_destroy(edges);
    return 0;
}
