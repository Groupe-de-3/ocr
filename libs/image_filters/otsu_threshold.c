#include <float.h>
#include "otsu_threshold.h"

static double intra_class_variant(ImageView *imgv, double threshold) {
    double pixel_count = (double)(imgv->width * imgv->height);

    double count_0 = 0., total_0 = 0.;
    double total_1 = 0.;
    for (int y = 0; y < (int)imgv->height; y++) {
        for (int x = 0; x < (int)imgv->width; x++) {
            double p = imgv_get_pixel_grayscale(imgv, x, y);
            if (p > threshold) {
                total_1 += p;
            }
            else {
                count_0 += 1;
                total_0 += p;
            }
        }
    }

    double mean_0 = total_0 / pixel_count;
    double mean_1 = total_1 / pixel_count;

    double weight_0 = count_0 / pixel_count;
    double weight_1 = 1. - weight_0;
    
    double a = mean_0 - mean_1;
    return weight_0 * weight_1 * a*a;
}

double otsus_method(ImageView *img) {
    double max_intensity = 0.;
    for (int y = 0; y < (int)img->height; y++) {
        for (int x = 0; x < (int)img->width; x++) {
            if (imgv_get_pixel_grayscale(img, x, y) > max_intensity)
                max_intensity = imgv_get_pixel_grayscale(img, x, y);
        }
    }

    double best_thresh;
    double best_intra_class_variance = DBL_MIN;
    for (double t = 0.; t < max_intensity; t += 0.001) {
        double v = intra_class_variant(img, t);
        if (v > best_intra_class_variance) {
            best_intra_class_variance = v;
            best_thresh = t;
        }
    }

    return best_thresh;
}

