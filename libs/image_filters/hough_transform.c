#include "hough_transform.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "vec.h"

void hough_acc_space_run_at(
    ImageView *in, ImageView *gradient_dir, ImageView *out, int x, int y
) {
    float pixel_value = (float)imgv_get_pixel_grayscale(in, x, y);
    if (pixel_value == 0.)
        return;
    float gd = (float)imgv_get_pixel_grayscale(gradient_dir, x, y);
    
    // Gradient direction but with 180 symmetry
    float line_gd = gd;
    if (line_gd < 0.f)
        line_gd = (float)M_PI + line_gd;

    float cx = (float)x - (float)in->width / 2.f;
    float cy = (float)y - (float)in->height / 2.f;

    float image_max_dist =
        sqrtf((float)(in->width * in->width + in->height * in->height)) / 2.f;

    for (int px = 0; px < out->width; px++) {
        float ptheta     = ((float)px / (float)(out->width - 1)) * (float)M_PI;
        float angle_dist = ptheta > line_gd ? ptheta - line_gd : line_gd - ptheta;
        if (angle_dist > M_PI_4)
            continue;

        double pdist = cx * sin(ptheta) + cy * cos(ptheta);
        int    py    = (int)round(
                  ((pdist + image_max_dist) / (image_max_dist * 2)) *
                  (float)(out->height - 1)
              );

        imgv_set_pixel_grayscale(
            out, px, py, imgv_get_pixel_grayscale(out, px, py) + pixel_value
        );
    }
}
void hough_acc_space_run(
    ImageView *in, ImageView *gradient_dir, ImageView *out
) {
    assert(in->width == gradient_dir->width);
    assert(in->height == gradient_dir->height);

    for (int y = 0; y < in->height; y++)
        for (int x = 0; x < in->width; x++)
            hough_acc_space_run_at(in, gradient_dir, out, x, y);

    grayscale_pixel_t max_val = 0.;
    for (int y = 0; y < out->height; y++)
        for (int x = 0; x < out->width; x++) {
            grayscale_pixel_t v = imgv_get_pixel_grayscale(out, x, y);
            if (v > max_val)
                max_val = v;
        }
    for (int y = 0; y < out->height; y++)
        for (int x = 0; x < out->width; x++)
            imgv_set_pixel_grayscale(
                out, x, y, imgv_get_pixel_grayscale(out, x, y) / max_val
            );
}
void hough_acc_space_probabilistic_run(
    ImageView *in, ImageView *gradient_dir, ImageView *out, size_t m
) {
    assert(in->width == gradient_dir->width);
    assert(in->height == gradient_dir->height);

    srand((unsigned int)time(NULL));
    for (size_t i = 0; i < m; i++) {
        int x = rand() % in->width;
        int y = rand() % in->height;
        hough_acc_space_run_at(in, gradient_dir, out, x, y);
    }

    grayscale_pixel_t max_val = 0.;
    for (int y = 0; y < out->height; y++)
        for (int x = 0; x < out->width; x++) {
            grayscale_pixel_t v = imgv_get_pixel_grayscale(out, x, y);
            if (v > max_val)
                max_val = v;
        }
    for (int y = 0; y < out->height; y++)
        for (int x = 0; x < out->width; x++)
            imgv_set_pixel_grayscale(
                out, x, y, imgv_get_pixel_grayscale(out, x, y) / max_val
            );
}

void hough_acc_space_draw_lines(ImageView *acc_space, ImageView *out) {
    double high_threshold = 0.1;

    float image_max_dist = sqrtf((float)(out->width*out->width + out->height*out->height)) / 2.f;
    for (int y = 0; y < (int)acc_space->height; y++) {
        for (int x = 0; x < (int)acc_space->width; x++) {
            float value = imgv_get_pixel_grayscale(acc_space, x, y);
            if (value < high_threshold)
                continue;

            float theta = ((float)x / (float)(acc_space->width - 1)) * (float)M_PI;
            float dist = ((float)y / (float)(acc_space->height - 1)) * image_max_dist*2 - image_max_dist;
            float stheta = sinf(theta);
            float ctheta = cosf(theta);

            if (theta < M_PI_4*3 && theta > M_PI_4) {
                for (int oy = 0; oy < (int)out->height; oy++) {
                    float cy = (float)oy - (float)out->height/2.f;
                    float cx = (dist - cy * ctheta) / stheta;
                    cx += (float)out->width / 2.f;
                    int ox = (int)floor(cx);
                    if (ox >= 0 && ox < (int)out->width) {
                        float a = imgv_get_pixel_grayscale(out, ox, oy);
                        imgv_set_pixel_grayscale(out, ox, oy, value + a);
                    }
                }
            }
            else {
                for (int ox = 0; ox < (int)out->width; ox++) {
                    float cx = (float)ox - (float)out->width/2.f;
                    float cy = (dist - cx * stheta) / ctheta;
                    cy += (float)out->height / 2.f;
                    int oy = (int)floor(cy);
                    if (oy >= 0 && oy < (int)out->height) {
                        float a = imgv_get_pixel_grayscale(out, ox, oy);
                        imgv_set_pixel_grayscale(out, ox, oy, value + a);
                    }
                }
            }
        }
    }

    float m = 0.;
    for (int y = 0; y < (int)out->height; y++) {
        for (int x = 0; x < (int)out->width; x++) {
            float v = imgv_get_pixel_grayscale(out, x, y);
            if (v > m)
                m = v;
        }
    }

    for (int y = 0; y < (int)out->height; y++) {
        for (int x = 0; x < (int)out->width; x++) {
            float v = imgv_get_pixel_grayscale(out, x, y);
            imgv_set_pixel_grayscale(out, x, y, v / m);
        }
    }
}
