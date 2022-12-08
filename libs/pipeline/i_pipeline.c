#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#include "i_pipeline.h"

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

static void compose_image(ImageView *from, ImageView *to, some_pixel_t icolor) {
    assert(from->width == to->width);
    assert(from->height == to->height);
    
    enum PixelFormat f = img_equivalent_float_channels_format(from->image->format);
    size_t channels = img_float_channels_length(f);
    any_pixel_t color = img_some_to_any(icolor, f);
    
    for (int x = 0; x < from->width; x++) {
        for (int y = 0; y < from->width; y++) {
            grayscale_pixel_t v = imgv_get_pixel_grayscale(from, x, y);
            any_pixel_t p = img_some_to_any(
                imgv_get_pixel_some(from, x, y), f
            );
            
            for (size_t c = 0; c < channels; c++) {
                p.float_channels[c] =
                    (1.f - v) * color.float_channels[c] +
                           v  * color.float_channels[c];
            }
        }
    }
}

PipelineIntermediaryResult pipeline_primary(ImageView *image_view) {
    size_t target_width =
        image_view->width > image_view->height ? 800 : (size_t)((image_view->width * 800) / image_view->height);
    size_t target_height =
        image_view->height > image_view->width ? 800 : (size_t)((image_view->height * 800) / image_view->width);

    Image     resized      = img_new(target_width, target_height, image_view->image->format);
    ImageView resized_view = imgv_default(&resized);
    resized_view.wraping_mode = WrappingMode_Clamp;

    bilinear_resize(image_view, &resized_view);

    // Bluring image
    Image blured =
        img_new(resized.width, resized.height, PixelFormat_GrayScale);
    ImageView blur_view    = imgv_default(&blured);
    blur_view.wraping_mode = WrappingMode_Clamp;

    // Executing blur
    gaussian_blur_run(&resized_view, &blur_view, 2.f);

    // Computing image's gradient
    Image     gradient = img_new(resized.width, resized.height, resized.format);
    ImageView gradient_view    = imgv_default(&gradient);
    gradient_view.wraping_mode = WrappingMode_Clamp;
    Image gradient_dir = img_new(resized.width, resized.height, resized.format);
    ImageView gradient_dir_view    = imgv_default(&gradient_dir);
    gradient_dir_view.wraping_mode = WrappingMode_Clamp;

    sobel_execute(&blur_view, &gradient_view, &gradient_dir_view);

    // Computing image's gradient
    Image edges = img_new(resized.width, resized.height, PixelFormat_GrayScale);
    ImageView edges_view    = imgv_default(&edges);
    edges_view.wraping_mode = WrappingMode_Clamp;

    CannyParameters canny_parameters = canny_guess_parameters(&gradient_view);
    canny_run(
        &gradient_view, &gradient_dir_view, &edges_view, canny_parameters
    );

    Image edges_mask =
        img_new(resized.width, resized.height, PixelFormat_GrayScale);
    ImageView edges_mask_view    = imgv_default(&edges_mask);
    edges_mask_view.wraping_mode = WrappingMode_Clamp;
    box_blur_run(&edges_view, &edges_mask_view, 11);
    global_threshold_run(&edges_mask_view, 0.09f);

    blood_fill_largest_blob(&edges_mask_view, false);
    image_mask_run(&edges_view, &edges_mask_view);

    // Computing image's gradient
    Image     hough         = img_new(2000, 2000, PixelFormat_GrayScale);
    ImageView hough_view    = imgv_default(&hough);
    hough_view.wraping_mode = WrappingMode_Clamp;

    hough_acc_space_run(&edges_view, &gradient_dir_view, &hough_view);

    HoughLine *hough_lines = hough_extract_lines(
        &hough_view, (int)resized.width, (int)resized.height
    );
    HoughLine *extrem_lines = hough_extract_extermum_lines(hough_lines);

    ParsedSudokuResult parse_rslt = sudoku_parse_from_lines(
        extrem_lines, resized_view.width, resized_view.height
    );

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
    img_destroy(resized);
    img_destroy(blured);
    img_destroy(gradient);
    img_destroy(gradient_dir);
    img_destroy(edges);
    img_destroy(hough);
    
    return (PipelineIntermediaryResult) {
        .sudoku_img = sudoku,
    };
}

PipelineSecondaryResult pipeline_seconday(PipelineIntermediaryResult *prslt) {
    Image sudoku = prslt->sudoku_img;
    ImageView sudoku_view = imgv_default(&sudoku);

    sudoku_view.wraping_mode = WrappingMode_Clamp;
    Image prep_sudoku = img_new(
        (size_t)sudoku_view.width, (size_t)sudoku_view.height,
        PixelFormat_GrayScale
    );
    ImageView prep_view = imgv_default(&prep_sudoku);
    prep_view.wraping_mode= WrappingMode_Clamp;
    imgv_copy(&sudoku_view, &prep_view);
    
    Image sudoku_cells = img_new(
        28 * 9, 28 * 9, PixelFormat_GrayScale
    );
    ImageView cells_view = imgv_default(&sudoku_cells);
    cells_view.wraping_mode= WrappingMode_Clamp;

    Matrix(ImageView) sudoku_imgs = m_new(ImageView, 9, 9);
    Matrix(bool) sudoku_mask = m_new(bool, 9, 9);

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
            m_get2(sudoku_mask, x, y) = false;
            if (v < 500000)
                continue;
            m_get2(sudoku_mask, x, y) = true;

            ImageView cell_view = {
                .image = &sudoku_cells,
                .wraping_mode = WrappingMode_Clamp,
                .x = start_x, .y = start_y,
                .width = 28, .height = 28
            };
            bilinear_resize(&original_view, &cell_view);

            m_get2(sudoku_imgs, x, y) = cell_view;
        }
    }
    
    bmp_save_to_path("SUDOKU_Z.bmp", &cells_view);

    neural_network NN = ia_load("ocr3.txt");
    Matrix(char) parsed_sudoku = m_new(char, 9, 9);
    memset(parsed_sudoku, 0, 9 * 9 * sizeof(char));
    
    ia_launch(NN, sudoku_mask, sudoku_imgs, parsed_sudoku);

    // sudoku_print(parsed_sudoku);
    
    Image digits;
    img_load_file("digits.bmp", &digits, false, PixelFormat_GrayScale);
    ImageView digits_view = imgv_default(&digits);

    int digit_width = (int)(
        ((digits.width * 80) / digits.height) / 9
    );

    Image resized_digits = img_new(
        (digits.width * 80) / digits.height, 80, PixelFormat_GrayScale
    );
    ImageView resized_digits_view = imgv_default(&resized_digits);
    bilinear_resize(&digits_view, &resized_digits_view);

    for (size_t x = 0; x < 9; x++) {
        for (size_t y = 0; y < 9; y ++) {
            ImageView in_view = (ImageView) {
                .image = &resized_digits,
                .height = 80,
                .width  = digit_width,
                .x = digit_width * m_get2(parsed_sudoku, x, y),
                .y = 0,
            };
            ImageView out_view = (ImageView) {
                .image  = &sudoku,
                .height = 80,
                .width  = digit_width,
                .x      = (int)(100 * x) + 50 - digit_width / 2,
                .y      = (int)(100 * y) + 10,
            };
            compose_image(
                &in_view, &out_view, (some_pixel_t) {
                    .value.rgbf = { 1., 0., 0. },
                    .format = PixelFormat_Rgbf,
                }
            );
        }
    }
    
    m_destroy(sudoku_imgs);
    ia_memory_free(&NN);
        

    return (PipelineSecondaryResult) {
        .result = sudoku,
        .sudoku = parsed_sudoku,
        .sudoku_mask = sudoku_mask,
    };
}
