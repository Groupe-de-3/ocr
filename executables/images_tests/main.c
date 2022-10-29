#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#include "gaussian_adaptive_threshold.h"
#include "image_mask.h"
#include "blood_filling.h"
#include "bilinear_sampling.h"
#include "global_threshold.h"
#include "box_blur.h"
#include "canny_edge_detector.h"
#include "gaussian_blur.h"
#include "hough_transform.h"
#include "images.h"
#include "img_formats/bmp.h"
#include "kernel_convolution.h"
#include "matrices.h"
#include "sobel_operator.h"
#include "vec.h"

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

    size_t target_width =
        image.width > image.height ? 800 : (image.width * 800) / image.height;
    size_t target_height =
        image.height > image.width ? 800 : (image.height * 800) / image.width;

    gettimeofday(&start, NULL);
    printf("Resizing image to %zu x %zu\n", target_width, target_height);
    Image     resized      = img_new(target_width, target_height, image.format);
    ImageView resized_view = imgv_default(&resized);
    resized_view.wraping_mode = WrappingMode_Clamp;

    bilinear_resize(&image_view, &resized_view);

    printf("    Saving to resized.bmp\n");
    bmp_save_to_path("resized.bmp", &resized);
    printf("    Done (%ldms)\n", timediff(start));

    if (false) {
        // Bluring image using box blur
        Image box_blur = img_new(resized.width, resized.height, resized.format);
        imgv_default(&box_blur);

        ImageView box_blur_view    = imgv_default(&box_blur);
        box_blur_view.wraping_mode = WrappingMode_Clamp;

        // Executing blur
        printf("Box Bluring image\n");
        box_blur_run(&resized_view, &box_blur_view, 11);

        // Saving blur
        bmp_save_to_path("box_blured.bmp", &box_blur);

        img_destroy(box_blur);
    }

    gettimeofday(&start, NULL);
    printf("Bluring and grayscaling image\n");

    // Bluring image
    Image blured =
        img_new(resized.width, resized.height, PixelFormat_GrayScale);
    ImageView blur_view    = imgv_default(&blured);
    blur_view.wraping_mode = WrappingMode_Clamp;

    // Executing blur
    gaussian_blur_run(&resized_view, &blur_view, 1.f);

    printf("    Saving blur to blured.bmp\n");
    // Saving blur
    bmp_save_to_path("blured.bmp", &blured);
    printf("    Done (%ldms)\n", timediff(start));
    
    gettimeofday(&start, NULL);
    printf("Computing gradient\n");

    // Computing image's gradient
    Image     gradient = img_new(resized.width, resized.height, resized.format);
    ImageView gradient_view    = imgv_default(&gradient);
    gradient_view.wraping_mode = WrappingMode_Clamp;
    Image gradient_dir = img_new(resized.width, resized.height, resized.format);
    ImageView gradient_dir_view    = imgv_default(&gradient_dir);
    gradient_dir_view.wraping_mode = WrappingMode_Clamp;

    sobel_execute(&blur_view, &gradient_view, &gradient_dir_view);
    printf("    Saving gradient to gradient.bmp\n");
    bmp_save_to_path("gradient.bmp", &gradient);
    printf("    Done (%ldms)\n", timediff(start));

    gettimeofday(&start, NULL);
    printf("Running Canny Edge Detector\n");
    // Computing image's gradient
    Image edges = img_new(resized.width, resized.height, PixelFormat_GrayScale);
    ImageView edges_view    = imgv_default(&edges);
    edges_view.wraping_mode = WrappingMode_Clamp;

    CannyParameters canny_parameters = canny_guess_parameters(&gradient_view);
    canny_run(
        &gradient_view, &gradient_dir_view, &edges_view, canny_parameters
    );
    
    printf("    Saving edges to edges.bmp\n");
    bmp_save_to_path("edges.bmp", &edges);
    printf("    Done (%ldms)\n", timediff(start));

    gettimeofday(&start, NULL);
    printf("Computing a mask for the edges\n");
    Image edges_mask = img_new(resized.width, resized.height, PixelFormat_GrayScale);
    ImageView edges_mask_view    = imgv_default(&edges_mask);
    edges_mask_view.wraping_mode = WrappingMode_Clamp;
    box_blur_run(&edges_view, &edges_mask_view, 9);
    printf("    Saving step one to edges-mask1.bmp\n");
    bmp_save_to_path("edges-mask1.bmp", &edges_mask);
    global_threshold_run(&edges_mask_view, 0.1f);
    printf("    Saving step two to edges-mask2.bmp\n");
    bmp_save_to_path("edges-mask2.bmp", &edges_mask);
    blood_fill_largest_blob(&edges_mask_view);
    printf("    Saving step three to edges-mask3.bmp\n");
    bmp_save_to_path("edges-mask3.bmp", &edges_mask);
    
    printf("    Applying the mask to the edges");
    image_mask_run(&edges_view, &edges_mask_view);
    printf("    Saving masked edges to masked-edges.bmp\n");
    bmp_save_to_path("masked-edges.bmp", &edges);

    printf("    Done (%ldms)\n", timediff(start));
    
    gettimeofday(&start, NULL);
    printf("Runnig Hough Transform\n");
    // Computing image's gradient
    Image     hough         = img_new(2000, 2000, PixelFormat_GrayScale);
    ImageView hough_view    = imgv_default(&hough);
    hough_view.wraping_mode = WrappingMode_Clamp;

    hough_acc_space_run(
        &edges_view, &gradient_dir_view, &hough_view
    );
    printf("    Saving edges to hough.bmp\n");
    bmp_save_to_path("hough.bmp", &hough);
    printf("    Done (%ldms)\n", timediff(start));

    gettimeofday(&start, NULL);
    printf("Drawing hough lines\n");
    // Computing image's gradient
    Image hough_lines_img =
        img_new(resized.width, resized.height, resized.format);
    ImageView hough_lines_img_view = imgv_default(&hough_lines_img);

    imgv_copy(&resized_view, &hough_lines_img_view);

    HoughLine *hough_lines = hough_extract_lines(
        &hough_view, (int)resized.width, (int)resized.height
    );
    printf("    Extracted %zu lines\n", vec_size(hough_lines));

    some_pixel_t line_colors = (some_pixel_t){
        .format = PixelFormat_Rgb8,
        .value  = {
                   .rgb8 =
                 (rgb8_pixel_t){
                     .r = 255,
                     .g = 0,
                     .b = 0,
                }, }
    };
    for (size_t i = 0; i < vec_size(hough_lines); i++) {
        hough_acc_space_draw_line(
            hough_lines[i], &hough_lines_img_view, line_colors, false
        );
    }
    printf("    Saving edges to hough-lines.bmp\n");
    bmp_save_to_path("hough-lines.bmp", &hough_lines_img);
    printf("    Done (%ldms)\n", timediff(start));

    vec_destroy(hough_lines);

    img_destroy(edges_mask);
    img_destroy(hough_lines_img);
    img_destroy(image);
    img_destroy(resized);
    img_destroy(blured);
    img_destroy(gradient);
    img_destroy(gradient_dir);
    img_destroy(edges);
    img_destroy(hough);
    return 0;
}
