#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#include "bilinear_sampling.h"
#include "otsu_threshold.h"
#include "blood_filling.h"
#include "box_blur.h"
#include "canny_edge_detector.h"
#include "gaussian_adaptive_threshold.h"
#include "gaussian_blur.h"
#include "global_threshold.h"
#include "hough_transform.h"
#include "image_mask.h"
#include "image_view.h"
#include "images.h"
#include "img_formats/bmp.h"
#include "kernel_convolution.h"
#include "sobel_operator.h"
#include "sudoku_parser.h"
#include "vec.h"

#include "sudoku.h"
#include "data.h"
#include "ia.h"
#include "matrices.h"
#include "save.h"
#include "mnist.h"
#include "struct.h"
#include "time.h"
#include "start.h"

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

    Image           image;
    ImageLoadResult rslt =
        img_load_file(argv[1], &image, false, PixelFormat_Rgbf);
    if (!rslt.success) {
        errx(1, "Could not load");
    }
    ImageView image_view    = imgv_default(&image);
    image_view.wraping_mode = WrappingMode_Clamp;

    size_t target_width =
        image.width > image.height ? 800 : (image.width * 800) / image.height;
    size_t target_height =
        image.height > image.width ? 800 : (image.height * 800) / image.width;

    gettimeofday(&start, NULL);
    Image     resized      = img_new(target_width, target_height, image.format);
    ImageView resized_view = imgv_default(&resized);
    resized_view.wraping_mode = WrappingMode_Clamp;

    bilinear_resize(&image_view, &resized_view);


    if (false) {
        // Bluring image using box blur
        Image box_blur = img_new(resized.width, resized.height, resized.format);
        imgv_default(&box_blur);

        ImageView box_blur_view    = imgv_default(&box_blur);
        box_blur_view.wraping_mode = WrappingMode_Clamp;

        // Executing blur

        box_blur_run(&resized_view, &box_blur_view, 11);


        img_destroy(box_blur);
    }

    gettimeofday(&start, NULL);

    // Bluring image
    Image blured =
        img_new(resized.width, resized.height, PixelFormat_GrayScale);
    ImageView blur_view    = imgv_default(&blured);
    blur_view.wraping_mode = WrappingMode_Clamp;

    // Executing blur
    gaussian_blur_run(&resized_view, &blur_view, 2.f);

    gettimeofday(&start, NULL);

    // Computing image's gradient
    Image     gradient = img_new(resized.width, resized.height, resized.format);
    ImageView gradient_view    = imgv_default(&gradient);
    gradient_view.wraping_mode = WrappingMode_Clamp;
    Image gradient_dir = img_new(resized.width, resized.height, resized.format);
    ImageView gradient_dir_view    = imgv_default(&gradient_dir);
    gradient_dir_view.wraping_mode = WrappingMode_Clamp;

    sobel_execute(&blur_view, &gradient_view, &gradient_dir_view);

    gettimeofday(&start, NULL);

    // Computing image's gradient
    Image edges = img_new(resized.width, resized.height, PixelFormat_GrayScale);
    ImageView edges_view    = imgv_default(&edges);
    edges_view.wraping_mode = WrappingMode_Clamp;

    CannyParameters canny_parameters = canny_guess_parameters(&gradient_view);
    canny_run(
        &gradient_view, &gradient_dir_view, &edges_view, canny_parameters
    );


    gettimeofday(&start, NULL);
    Image edges_mask =
        img_new(resized.width, resized.height, PixelFormat_GrayScale);
    ImageView edges_mask_view    = imgv_default(&edges_mask);
    edges_mask_view.wraping_mode = WrappingMode_Clamp;
    box_blur_run(&edges_view, &edges_mask_view, 11);
    global_threshold_run(&edges_mask_view, 0.09f);

    blood_fill_largest_blob(&edges_mask_view, false);


    image_mask_run(&edges_view, &edges_mask_view);



    gettimeofday(&start, NULL);

    // Computing image's gradient
    Image     hough         = img_new(2000, 2000, PixelFormat_GrayScale);
    ImageView hough_view    = imgv_default(&hough);
    hough_view.wraping_mode = WrappingMode_Clamp;

    hough_acc_space_run(&edges_view, &gradient_dir_view, &hough_view);

    Image hough_all_lines_img =
        img_new(resized.width, resized.height, resized.format);
    ImageView hough_all_lines_img_view = imgv_default(&hough_all_lines_img);

    hough_acc_space_draw_all_lines(&hough_view, &hough_all_lines_img_view);



    gettimeofday(&start, NULL);

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

    draw_all_lines_on(&hough_peak_lines_img_view, hough_lines);


    HoughLine *extrem_lines = hough_extract_extermum_lines(hough_lines);

    draw_all_lines_on(&hough_lines_img_view, extrem_lines);

    ParsedSudokuResult parse_rslt = sudoku_parse_from_lines(
        extrem_lines, resized_view.width, resized_view.height
    );

    gettimeofday(&start, NULL);

    Image     sudoku      = img_new(900, 900, resized.format);
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

    sudoku_view.wraping_mode = WrappingMode_Clamp;
    Image prep_sudoku = img_new(
        (size_t)sudoku_view.width, (size_t)sudoku_view.height,
        PixelFormat_GrayScale
    );
    ImageView prep_view = imgv_default(&prep_sudoku);
    prep_view.wraping_mode= WrappingMode_Clamp;
    imgv_copy(&sudoku_view, &prep_view);
    
    bmp_save_to_path("SUDOKU_A.bmp", &prep_view);
    // bmp_save_to_path("SUDOKU_B.bmp", &prep_view);
    
    Image sudoku_cells = img_new(
        28 * 9, 28 * 9, PixelFormat_GrayScale
    );
    ImageView cells_view = imgv_default(&sudoku_cells);
    cells_view.wraping_mode= WrappingMode_Clamp;

    Matrix(ImageView) sudoku_imgs = m_new(ImageView, 9, 9);
    for (size_t x = 0; x < 9; x += 1) {
        int original_start_x = (int)(x * prep_sudoku.width) / 9;
        int start_x = (int)(x * 28 * 9) / 9;
        
        for (size_t y = 0; y < 9; y += 1) {
            int original_start_y = (int)(y * prep_sudoku.height) / 9;
            int start_y = (int)(y * 28 * 9) / 9;

            // printf("start   : %dx%d\n", start_x, start_y);
            // printf("og start: %dx%d\n", original_start_x, original_start_y);

            ImageView original_view = {
                .image = &prep_sudoku,
                .wraping_mode = WrappingMode_Clamp,
                .x = original_start_x, .y = original_start_y,
                .width = sudoku_view.width/9, .height = sudoku_view.height/9
            };

            double thresh = otsus_method(&original_view);
            size_t white_pixs = global_threshold_run(&original_view, (float)thresh);
            // Inverse if there is more white than blacks
            if ((int)white_pixs > (original_view.width * original_view.height) / 2) {
                for (int sx = 0; sx < original_view.width; sx++)
                    for (int sy = 0; sy < original_view.height; sy++)
                        imgv_set_pixel_grayscale(
                            &original_view, sx, sy,
                            1.f - imgv_get_pixel_grayscale(&original_view, sx, sy)
                        );
            }
            
            long v = blood_fill_largest_weighted_blob(&original_view, blood_fill_center_weighter, true);
            if (v < 500000)
                continue;

            ImageView cell_view = {
                .image = &sudoku_cells,
                .wraping_mode = WrappingMode_Clamp,
                .x = start_x, .y = start_y,
                .width = 28, .height = 28
            };
            bilinear_resize(&original_view, &cell_view);

            char bjr[100];
            snprintf(bjr, 100, "%02zux%02zu.bmp", x, y);
            // printf("Start %s\n", bjr);
            bmp_save_to_path(bjr, &cell_view);
            // printf("End %s\n", bjr);
            m_get2(sudoku_imgs, x, y) = cell_view;
        }
    }
    
    bmp_save_to_path("SUDOKU_Z.bmp", &cells_view);
    return 0;

    neural_network NN = ia_load("ocr40.txt");
    
    char * sudoku__ = ia_launch(NN, sudoku_imgs);

    //char * sudoku__ = calloc(81 ,sizeof(char));

    //solve(0, 0, sudoku__);

    sudoku_print(sudoku__);

    free(sudoku__);
    ia_memory_free(&NN);
    img_destroy(sudoku);
    return 0;
}
