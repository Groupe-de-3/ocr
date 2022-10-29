#include "hough_transform.h"

#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "box_blur.h"
#include "images.h"
#include "vec.h"

static float get_image_max_dist(int out_width, int out_height) {
    return sqrtf((float)(out_width * out_width + out_height * out_height)) /
           2.f;
}

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

    float image_max_dist = get_image_max_dist(in->width, in->height);

    for (int px = 0; px < out->width; px++) {
        float ptheta = ((float)px / (float)(out->width - 1)) * (float)M_PI;
        float angle_dist =
            ptheta > line_gd ? ptheta - line_gd : line_gd - ptheta;
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

static void hough_get_image_max(
    ImageView *space, int *max_x, int *max_y, float *max_value
) {
    *max_x     = -1;
    *max_y     = -1;
    *max_value = FLT_MIN;

    for (int y = 0; y < space->height; y++) {
        for (int x = 0; x < space->width; x++) {
            float v = (float)imgv_get_pixel_grayscale(space, x, y);
            if (v > *max_value) {
                *max_value = v;
                *max_x     = x;
                *max_y     = y;
            }
        }
    }
}

static void hough_clear_pixel(ImageView *space, int x, int y) {
    for (int dy = -50; dy <= 50; dy++) {
        for (int dx = -20; dx <= 20; dx++) {
            if (!imgv_in_bound(space, x + dx, y + dy))
                continue;
            imgv_set_pixel_any(
                space, x + dx, y + dy, ANY_BLACK(space->image->format)
            );
        }
    }
}

static HoughLine
hough_pixel_as_line(ImageView *space, float image_max_dist, int x, int y) {
    float dist = ((float)y / (float)(space->height - 1)) * image_max_dist * 2 -
                 image_max_dist;
    float theta = ((float)x / (float)(space->width - 1)) * (float)M_PI;

    return (HoughLine){
        .r     = dist,
        .theta = theta,
    };
}

HoughLine *hough_extract_lines(
    ImageView *space, int target_output_width, int target_output_height
) {
    // Percentage of the maximum value that the minimum line should have
    const float stop_dip      = 0.35f;
    const float min_threshold = 0.1f;

    Image blured_space = img_new(
        (size_t)space->width, (size_t)space->height, space->image->format
    );
    ImageView blured_view = imgv_default(&blured_space);
    box_blur_run(space, &blured_view, 3);

    HoughLine *lines = vec_new(HoughLine);
    float      image_max_dist =
        get_image_max_dist(target_output_width, target_output_height);

    int   max_x, max_y;
    float max_value;
    // This first maximum is added to the vec in the first iterator of the loop
    hough_get_image_max(&blured_view, &max_x, &max_y, &max_value);

    int   current_mx = max_x, current_my = max_y;
    float cm = max_value;
    while (cm >= stop_dip * max_value && cm >= min_threshold) {
        hough_clear_pixel(&blured_view, current_mx, current_my);
        *vec_push(&lines, HoughLine) = hough_pixel_as_line(
            &blured_view, image_max_dist, current_mx, current_my
        );

        hough_get_image_max(&blured_view, &current_mx, &current_my, &cm);
    }

    img_destroy(blured_space);
    return lines;
}

void hough_acc_space_draw_line(
    HoughLine line, ImageView *out, some_pixel_t color, bool additive
) {
    float theta = line.theta;
    float dist  = line.r;

    float stheta = sinf(theta);
    float ctheta = cosf(theta);

    if (theta < M_PI_4 * 3 && theta > M_PI_4) {
        for (int oy = 0; oy < (int)out->height; oy++) {
            float cy = (float)oy - (float)out->height / 2.f;
            float cx = (dist - cy * ctheta) / stheta;
            cx += (float)out->width / 2.f;
            int ox = (int)floor(cx);
            if (ox >= 0 && ox < (int)out->width) {
                some_pixel_t a = SOME_BLACK(color.format);
                if (additive)
                    a = imgv_get_pixel_some(out, ox, oy);
                imgv_set_pixel_some(out, ox, oy, img_color_sum(a, color));
            }
        }
    }
    else {
        for (int ox = 0; ox < (int)out->width; ox++) {
            float cx = (float)ox - (float)out->width / 2.f;
            float cy = (dist - cx * stheta) / ctheta;
            cy += (float)out->height / 2.f;
            int oy = (int)floor(cy);
            if (oy >= 0 && oy < (int)out->height) {
                some_pixel_t a = SOME_BLACK(color.format);
                if (additive)
                    a = imgv_get_pixel_some(out, ox, oy);
                imgv_set_pixel_some(out, ox, oy, img_color_sum(a, color));
            }
        }
    }
}
void hough_acc_space_draw_all_lines(ImageView *acc_space, ImageView *out) {
    double high_threshold = 0.1;

    float image_max_dist = get_image_max_dist(out->width, out->height);
    for (int y = 0; y < (int)acc_space->height; y++) {
        for (int x = 0; x < (int)acc_space->width; x++) {
            some_pixel_t value = imgv_get_pixel_some(acc_space, x, y);
            if (img_some_to_grayscale(value) < high_threshold)
                continue;

            float theta =
                ((float)x / (float)(acc_space->width - 1)) * (float)M_PI;
            float dist = ((float)y / (float)(acc_space->height - 1)) *
                             image_max_dist * 2 -
                         image_max_dist;

            hough_acc_space_draw_line(
                (HoughLine){
                    .r     = dist,
                    .theta = theta,
                },
                out, value, true
            );
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

HoughLine *hough_extract_extermum_lines(HoughLine *lines) {
    const float angle_max_d = (float)M_PI_4;

    float *counts = vec_new(float);
    float *angles = vec_new(float);
    // Always twice a long as angles
    HoughLine *extremes = vec_new(HoughLine);
    
    for (size_t i = 0; i < vec_size(lines); i++) {
        float wrapped_theta = lines[i].theta;
        float wrapped_dist = lines[i].r;

        size_t angle_i = 0;
        for (; angle_i < vec_size(angles); angle_i++) {
            float angle_dist = fabsf(wrapped_theta - (angles[angle_i] / counts[angle_i]));
            if (angle_dist > (float)M_PI_2)
                angle_dist = (float)M_PI - angle_dist;

            if (angle_dist < angle_max_d) {
                //angles[angle_i] += wrapped_theta;
                //counts[angle_i] += 1;

                if (wrapped_dist < extremes[angle_i*2].r)
                    extremes[angle_i*2] = lines[i];
                else if (wrapped_dist > extremes[angle_i*2+1].r)
                    extremes[angle_i*2+1] = lines[i];
                break;
            }
        }

        // No matching angle was found
        if (angle_i == vec_size(angles)) {
            *vec_push(&counts, float) = 1;
            *vec_push(&angles, float) = wrapped_theta;
            *vec_push(&extremes, HoughLine) = lines[i];
            *vec_push(&extremes, HoughLine) = lines[i];
        }
    }
    
    vec_destroy(counts);
    vec_destroy(angles);
    return extremes;
}
