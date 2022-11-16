#pragma once

#include "image_view.h"

typedef struct {
    float r;
    float theta;
} HoughLine;

/*! \brief Creates the sinusoidal line from the x/y point in the hough transform
 *
 * \param in the sudoku image.
 * \param gradient_dir image as a matrice giving the gradient direction.
 * \param out the hough space.
 * \param x the point x coordinate.
 * \param y the point y coordinate.
 */
void hough_acc_space_run_at(
    ImageView *in, ImageView *gradient_dir, ImageView *out, int x, int y
);

/*! \brief Uses #hough_acc_space_run_at and goes through the image to create the
 * hough space. Also also normalizes theses values.
 *
 * \param in the sudoku image.
 * \param gradient_dir image as a matrice giving the gradient direction.
 * \param out the hough space.
 */
void hough_acc_space_run(
    ImageView *in, ImageView *gradient_dir, ImageView *out
);

/*! \brief Another way to create the hough space by going through random
 * coordinates
 *
 * \param[in] in the sudoku image.
 * \param[in] gradient_dir image as a matrice giving the gradient direction.
 * \param[out] out the hough space.
 * \param[in] m the number of pixel to choose.
 */
void hough_acc_space_probabilistic_run(
    ImageView *in, ImageView *gradient_dir, ImageView *out, size_t m
);

/*! \brief Uses an heuristic to find the best lines in the hough space.
 *
 * \param space the hough space.
 * \param target_output_width the width of the given image.
 * \param target_output_height the height of the given image.
 * \return the vec of the extracted lines.
 */
HoughLine *hough_extract_lines(
    ImageView *space, int target_output_width, int target_output_height
);

void hough_acc_space_draw_line(
    HoughLine line, ImageView *out, some_pixel_t color, _Bool additive
);

void hough_acc_space_draw_all_lines(ImageView *acc_space, ImageView *out);

/*! \brief Gives the most external lines.
 *
 * \param lines a vec of all the lines.
 * \return a new vec of all the external lines.
 */
HoughLine *hough_extract_extermum_lines(HoughLine *lines);
