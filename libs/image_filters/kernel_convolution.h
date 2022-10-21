#pragma once
#include "image_view.h"

/*! \brief Applies a Kernel at a specific pixel on an Image.
 *
 *  \param[in] src The image view from which to take pixel values.
 *  \param[in] kernel The kernel matrix (see #matrices.h). It must have 2
 * dimensions of odd sizes.
 *  \param[in] x The pixel's x coordinate.
 *  \param[in] y The pixel's y coordinate.
 *  \returns The computed value.
 */
grayscale_pixel_t
filter_kernel_run_at(ImageView *src, float *kernel, int x, int y);
/*! \brief Applies a Kernel convolution on the entire image.
 *
 *  This calls #filter_run_kernel_at on every pixel of src, storing the result
 * into out.
 *
 *  \param src The image view from which to take *grayscale* pixel values.
 *  \param out Image from which to write the computed pixel values. Must be the
 * same size of src. \param kernel The kernel matrix (see #matrices.h). It must
 * have 2 dimensions of odd sizes.
 */
void filter_kernel_run(ImageView *src, ImageView *out, float *kernel);
