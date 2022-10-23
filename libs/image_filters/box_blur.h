#pragma once

#include "image_view.h"

/*! \brief Executed a box blur on an image.
 *
 *  \param[in] in The image from which to get pixels.
 *  \param[out] out Where the blured image will be saved to, must be of the same
 * size as #in. \param size The size of the box blur that should be applied.
 * Must be odd.
 */
void box_blur_run(ImageView *in, ImageView *out, size_t size);
