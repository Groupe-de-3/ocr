#include <stddef.h>

#include "image_view.h"

/*! \brief Applies a gaussian blur to an image.
 *
 *  \param[in] in The image from which to take pixel data.
 *  \param[out] out Where to write the blured image data, must be of the same
 * size as #in.
 *  \param std_deriv The standard deviation of the blur. The higher
 * it gets, the blurier the image will get.
 */
void gaussian_blur_run(ImageView *in, ImageView *out, float std_deriv);
