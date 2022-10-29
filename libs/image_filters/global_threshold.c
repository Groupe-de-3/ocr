#include "global_threshold.h"

void global_threshold_run(ImageView *img, float threshold) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            if (imgv_get_pixel_grayscale(img, x, y) < threshold) {
                imgv_set_pixel_grayscale(img, x, y, GRAYSCALE_BLACK);
            }
            else {
                imgv_set_pixel_grayscale(img, x, y, GRAYSCALE_WHITE);
            }
        }
    }
}
