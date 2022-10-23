#include "hough_transform.h"

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

void hough_acc_space_run_at(
    ImageView *in, ImageView *gradient_dir, ImageView *out, int x, int y
) {
    float pixel_value    = (float)imgv_get_pixel_grayscale(in, x, y);
    if (pixel_value == 0.)
        return;
    float gd             = (float)imgv_get_pixel_grayscale(gradient_dir, x, y);

    float cx = (float)x - (float)in->width / 2.f;
    float cy = (float)y - (float)in->height / 2.f;
    
    double image_max_dist =
        sqrt((double)(in->width*in->width + in->height*in->height)) / 2.;

    for (int px = 0; px < out->width; px++) {
        double ptheta = ((double)px / (double)(out->width - 1)) * M_PI;
        double angle_dist = fmod(fabs(ptheta - gd), 2*M_PI);
        if (angle_dist > M_PI_4)
            continue;

        double pdist = cx*sin(ptheta) + cy*cos(ptheta);
        int py = (int)round(((pdist + image_max_dist) / (image_max_dist*2))
                * (double)(out->height - 1));
        
        imgv_set_pixel_grayscale(out, px, py,
            imgv_get_pixel_grayscale(out, px, py) + pixel_value);
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
            imgv_set_pixel_grayscale(out, x, y, imgv_get_pixel_grayscale(out, x, y) / max_val);
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
            imgv_set_pixel_grayscale(out, x, y, imgv_get_pixel_grayscale(out, x, y) / max_val);
}
