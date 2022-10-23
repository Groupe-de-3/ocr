#include "canny_edge_detector.h"

#include <math.h>
#include <stdbool.h>
#include <float.h>

#include "vec.h"

static bool feq(double a, double b) {
    return fabs(a - b) <= DBL_EPSILON;
}

static void extend_edges(ImageView *canny, ImageView *gradient_directions) {
    int w = (int)canny->width;
    int h = (int)canny->height;

    struct Pixel { int x; int y; double strong_direction; };
    struct Pixel *pixels = vec_new(struct Pixel);

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            while (vec_size(pixels) > 0) {
                struct Pixel pixel = vec_pop(pixels, struct Pixel);
                imgv_set_pixel_grayscale(canny, pixel.x, pixel.y, 1.);

                for (int dy = -1; dy <= 1; dy++) {
                    int py = pixel.y + dy;
                    for (int dx = -1; dx <= 1; dx++) {
                        int px = pixel.x + dx;
                        if (py == pixel.y && px == pixel.x)
                            continue;

                        // % for negative value isn't like a mathematic remainder
                        px = ((px % w) + w) % w;
                        py = ((py % h) + h) % h;
                        double g = imgv_get_pixel_grayscale(canny, px, py);
                        double d = imgv_get_pixel_grayscale(gradient_directions, px, py);
                        double angle_dist = fmod(fabs(d - pixel.strong_direction), 2*M_PI);
                        if (angle_dist > M_PI)
                            angle_dist = 2*M_PI - angle_dist;
                        if (
                            angle_dist < M_PI_4 &&
                            (feq(g, 0.25) || feq(g, 0.5))
                        ) {
                            *vec_push(&pixels, struct Pixel) = (struct Pixel) {
                                .x = px, .y = py, .strong_direction = d
                            };
                        }
                    }
                }
            }

            if (feq(imgv_get_pixel_grayscale(canny, x, y), 0.5)) {
                double direction = imgv_get_pixel_grayscale(gradient_directions, x, y);
                *vec_push(&pixels, struct Pixel) = (struct Pixel) {
                    .x = x, .y = y, .strong_direction = direction,
                };
            }
        }
    }
    
    vec_destroy(pixels);
}

void canny_run(ImageView *gradient, ImageView *gradient_direction, ImageView *out) {
    double max_gradient = 0.;
    for (int y = 0; y < (int)gradient->height; y++) {
        for (int x = 0; x < (int)gradient->width; x++) {
            double t = imgv_get_pixel_grayscale(gradient, x, y);
            if (t > max_gradient)
                max_gradient = t;
        }
    }
    
    double high_threshold = max_gradient / 2.5;
    double low_threshold = high_threshold / 3.;

    for (int y = 0; y < gradient_direction->height; y++) {
        for (int x = 0; x < gradient_direction->width; x++) {
            float d = (float)imgv_get_pixel_grayscale(gradient_direction, x, y);
            float g = (float)imgv_get_pixel_grayscale(gradient, x, y);

            int dx = (int)round(sinf(d));
            int dy = (int)round(cosf(d));

            double left_gradient = (double)imgv_get_pixel_grayscale(gradient, x + dx, y + dy);
            double right_gradient = (double)imgv_get_pixel_grayscale(gradient, x - dx, y - dy);

            if (g > left_gradient && g > right_gradient) {
                if (g >= high_threshold)
                    imgv_set_pixel_grayscale(out, x, y, 0.5);
                else if (g >= low_threshold)
                    imgv_set_pixel_grayscale(out, x, y, 0.25);
                else
                    imgv_set_pixel_grayscale(out, x, y, 0.);
            }
            else
                imgv_set_pixel_grayscale(out, x, y, 0.);
        }
    }
    
    extend_edges(out, gradient_direction);

    for (int y = 0; y < out->height; y++) {
        for (int x = 0; x < out->width; x++) {
            if (imgv_get_pixel_grayscale(out, x, y) < 0.5)
                imgv_set_pixel_grayscale(out, x, y, 0.);
        }
    }
}
