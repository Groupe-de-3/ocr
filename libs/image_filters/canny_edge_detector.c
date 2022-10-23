#include "canny_edge_detector.h"

#include <math.h>

void canny_run(ImageView *gradient, ImageView *gradient_direction, ImageView *out) {
    double max_gradient = 0.;
    for (int y = 0; y < (int)gradient->height; y++) {
        for (int x = 0; x < (int)gradient->width; x++) {
            double t = imgv_get_pixel_grayscale(gradient, x, y);
            if (t > max_gradient)
                max_gradient = t;
        }
    }
    
    double high_threshold = max_gradient / 3.;
    double low_threshold = high_threshold / 3.;

    for (int y = 0; y < (int)gradient_direction->height; y++) {
        for (int x = 0; x < (int)gradient_direction->width; x++) {
            float d = (float)imgv_get_pixel_grayscale(gradient_direction, x, y);
            float g = (float)imgv_get_pixel_grayscale(gradient, x, y);

            int dx = (int)round(sinf(d));
            int dy = (int)round(cosf(d));

            double left_gradient = imgv_get_pixel_grayscale(gradient, x + dx, y + dy);
            double right_gradient = imgv_get_pixel_grayscale(gradient, x - dx, y - dy);

            if (g > left_gradient && g > right_gradient) {
                if (g >= high_threshold)
                    imgv_set_pixel_grayscale(out, x, y, 1.);
                else if (g >= low_threshold)
                    imgv_set_pixel_grayscale(out, x, y, 0.25);
                else
                    imgv_set_pixel_grayscale(out, x, y, 0.);
            }
            else
                imgv_set_pixel_grayscale(out, x, y, 0.);
        }
    }
}
