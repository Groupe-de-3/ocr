#include <stddef.h>

/*! \brief Computes the optimal size for a gaussian blur kernel based on it's
 * bluring parameter.
 *
 *  \param param The standard deviation of the wanted gaussian blur kernel.
 *  \returns The optimal size the gaussian blur kernel of the given std
 * deviation should be. Or... how "blury" the image should get.
 */
size_t gaussian_blur_optimal_kernel_size(float param);
/*! \brief Fills in the given matrix with values of a gaussian blur kernel.
 *
 *  \param kernel_mat A 2D Matrix (see #matrices.h) with dimensions of off
 * sizes. The size may be computed using #gaussian_blur_optimal_kernel_size.
 *  \param param The The standard deviation of the gaussian distribution. The
 * higher it gets, the bluerier the image will get.
 */
void populate_gaussian_blur_kernel(float *kernel_mat, float param);
