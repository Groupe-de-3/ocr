#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#include "bilinear_sampling.h"
#include "blood_filling.h"
#include "box_blur.h"
#include "canny_edge_detector.h"
#include "gaussian_adaptive_threshold.h"
#include "gaussian_blur.h"
#include "global_threshold.h"
#include "hough_transform.h"
#include "image_mask.h"
#include "images.h"
#include "img_formats/bmp.h"
#include "kernel_convolution.h"
#include "matrices.h"
#include "sobel_operator.h"
#include "sudoku_parser.h"
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

static void draw_all_lines_on(ImageView *in, HoughLine *lines) {
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
    for (size_t i = 0; i < vec_size(lines); i++) {
        hough_acc_space_draw_line(lines[i], in, line_colors, false);
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
    Image edges_mask =
        img_new(resized.width, resized.height, PixelFormat_GrayScale);
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

    printf("    Applying the mask to the edges\n");
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

    hough_acc_space_run(&edges_view, &gradient_dir_view, &hough_view);
    printf("    Saving edges to hough.bmp\n");
    bmp_save_to_path("hough.bmp", &hough);

    Image hough_all_lines_img =
        img_new(resized.width, resized.height, resized.format);
    ImageView hough_all_lines_img_view = imgv_default(&hough_all_lines_img);

    hough_acc_space_draw_all_lines(&hough_view, &hough_all_lines_img_view);
    printf("    Saving all lines to hough-all-lines.bmp\n");
    bmp_save_to_path("hough-all-lines.bmp", &hough_all_lines_img);

    printf("    Done (%ldms)\n", timediff(start));

    gettimeofday(&start, NULL);
    printf("Extracting hough lines\n");
    Image hough_peak_lines_img =
        img_new(resized.width, resized.height, resized.format);
    ImageView hough_peak_lines_img_view = imgv_default(&hough_peak_lines_img);

    imgv_copy(&resized_view, &hough_peak_lines_img_view);

    Image hough_lines_img =
        img_new(resized.width, resized.height, resized.format);
    ImageView hough_lines_img_view = imgv_default(&hough_lines_img);

    imgv_copy(&resized_view, &hough_lines_img_view);

    HoughLine *hough_lines = hough_extract_lines(
        &hough_view, (int)resized.width, (int)resized.height
    );
    printf("    Extracted %zu lines\n", vec_size(hough_lines));

    draw_all_lines_on(&hough_peak_lines_img_view, hough_lines);
    printf("    Saving peak lines to hough-peak-lines.bmp\n");
    bmp_save_to_path("hough-peak-lines.bmp", &hough_peak_lines_img);

    HoughLine *extrem_lines = hough_extract_extermum_lines(hough_lines);
    printf("    Extracted %zu extrem lines\n", vec_size(extrem_lines));

    draw_all_lines_on(&hough_lines_img_view, extrem_lines);
    printf("    Saving lines to hough-lines.bmp\n");
    bmp_save_to_path("hough-lines.bmp", &hough_lines_img);
    printf("    Done (%ldms)\n", timediff(start));

    gettimeofday(&start, NULL);
    printf("Extracting the individuel sudoku cells\n");
    Image sudoku_corners_img =
        img_new(resized.width, resized.height, resized.format);
    ImageView sudoku_corners_img_view = imgv_default(&sudoku_corners_img);

    imgv_copy(&resized_view, &sudoku_corners_img_view);

    ParsedSudokuResult parse_rslt = sudoku_parse_from_lines(
        extrem_lines, sudoku_corners_img_view.width,
        sudoku_corners_img_view.height
    );
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
    for (int d = -5; d <= 5; d++) {
        {
            ipoint2d_t corners[4] = {
                parse_rslt.shape.a, parse_rslt.shape.b, parse_rslt.shape.c,
                parse_rslt.shape.d};
            for (int i = 0; i < 4; i++) {
                ipoint2d_t corner = corners[i];
                if (imgv_in_bound(
                        &sudoku_corners_img_view, corner.x + d, corner.y
                    ))
                    imgv_set_pixel_some(
                        &sudoku_corners_img_view, corner.x + d, corner.y,
                        line_colors
                    );
                if (imgv_in_bound(
                        &sudoku_corners_img_view, corner.x, corner.y + d
                    ))
                    imgv_set_pixel_some(
                        &sudoku_corners_img_view, corner.x, corner.y + d,
                        line_colors
                    );
            }
        }

        for (int j = 0; j < 9 * 9; j++) {
            iquadrilateral_t cell       = parse_rslt.cells[j];
            ipoint2d_t       corners[4] = {cell.a, cell.b, cell.c, cell.d};
            for (int i = 0; i < 4; i++) {
                ipoint2d_t corner = corners[i];
                if (imgv_in_bound(
                        &sudoku_corners_img_view, corner.x + d, corner.y
                    ))
                    imgv_set_pixel_some(
                        &sudoku_corners_img_view, corner.x + d, corner.y,
                        line_colors
                    );
                if (imgv_in_bound(
                        &sudoku_corners_img_view, corner.x, corner.y + d
                    ))
                    imgv_set_pixel_some(
                        &sudoku_corners_img_view, corner.x, corner.y + d,
                        line_colors
                    );
            }
        }
    }

    printf("    Saving edges to sudoku-corners.bmp\n");
    bmp_save_to_path("sudoku-corners.bmp", &sudoku_corners_img);
    printf("    Done (%ldms)\n", timediff(start));

    gettimeofday(&start, NULL);
    printf("Isolating the suodku\n");
    Image     sudoku      = img_new(800, 800, resized.format);
    ImageView sudoku_view = imgv_default(&sudoku);

    bilinear_perspective_transmute(
        &resized_view, i2fquadrilateral(parse_rslt.shape), &sudoku_view,
        (fquadrilateral_t){
            .a = fpoint2d(0, 0),
            .b = fpoint2d((float)(sudoku_view.width - 1), 0),
            .c = fpoint2d(
                (float)(sudoku_view.width - 1), (float)(sudoku_view.height - 1)
            ),
            .d = fpoint2d(0, (float)(sudoku_view.height - 1)),
        }
    );

    printf("    Saving edges to sudoku.bmp\n");
    bmp_save_to_path("sudoku.bmp", &sudoku);
    printf("    Done (%ldms)\n", timediff(start));

    vec_destroy(extrem_lines);
    vec_destroy(hough_lines);

    img_destroy(edges_mask);
    img_destroy(hough_all_lines_img);
    img_destroy(hough_peak_lines_img);
    img_destroy(hough_lines_img);
    img_destroy(image);
    img_destroy(resized);
    img_destroy(blured);
    img_destroy(gradient);
    img_destroy(gradient_dir);
    img_destroy(edges);
    img_destroy(hough);
    img_destroy(sudoku_corners_img);
    img_destroy(sudoku);
    return 0;
}
