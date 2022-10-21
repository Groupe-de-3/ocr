#pragma once
#include "image_view.h"

/*! \brief Executes the Sobel operator getting the gradient and gradient
 * direction of the image.
 *
 *  \param[in] in The input #ImageView, must not be NULL
 *  \param[out] gradient_out If not NULL, the #ImageView will be populated with
 * with computed gradient of each pixel in the input image. Ignored if NULL.
 *                           Must be of the same size as the Input.
 *  \param[out] gradient_dir_out If not NULL, the #ImageView will be populated
 * with with computed gradient direction in radians of each pixel in the input
 * image. Ignored if NULL. Must be of the same size as the Input.
 */
void sobel_execute(
    ImageView *in, ImageView *gradient_out, ImageView *gradient_dir_out
);
