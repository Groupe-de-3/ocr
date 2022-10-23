#include "bilinear_sampling.h"

#include "utils_math.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

some_pixel_t bilinear_sample(ImageView *img, float x, float y) {
    // Image must be in a "float channel" format
    size_t float_channels = img_float_channels_length(img->image->format);
    assert(float_channels != 0);

    // Splitting fractional and int part of the coordinates
    int xI = (int)x;
    int yI = (int)y;
    float xF = x - (float)xI;
    float yF = y - (float)yI;

    int x_plus = xI + 1;
    int y_plus = yI + 1;

    float top_value[float_channels];
    {
        any_pixel_t a = imgv_get_pixel_any(img, xI, yI);
        any_pixel_t b = imgv_get_pixel_any(img, x_plus, yI);
        for (size_t c = 0; c < float_channels; c++) {
            top_value[c] = a.float_channels[c] * (1.f - xF)
                         + b.float_channels[c] * xF;
        }
    }
    float bottom_value[float_channels];
    {
        any_pixel_t a = imgv_get_pixel_any(img, xI, y_plus);
        any_pixel_t b = imgv_get_pixel_any(img, x_plus, y_plus);
        for (size_t c = 0; c < float_channels; c++) {
            bottom_value[c] = a.float_channels[c] * (1.f - xF)
                         + b.float_channels[c] * xF;
        }
    }
    float final_value[float_channels];
    for (size_t c = 0; c < float_channels; c++) {
        final_value[c] = top_value[c] * (1.f - yF)
                       + bottom_value[c] * yF;
        final_value[c] = clamp(final_value[c], 0.f, 1.f);
    }
    
    some_pixel_t out = { .format = img->image->format };
    memcpy(out.value.float_channels, final_value, sizeof(float) * float_channels);
    return out;
}

void bilinear_resize(ImageView *from, ImageView *to) {
    float x_scale = (float)from->width / (float)to->width;
    float y_scale = (float)from->height / (float)to->height;
    for (int y = 0; y < to->height; y++) {
        for (int x = 0; x < to->width; x++) {
            imgv_set_pixel_some(to, x, y, bilinear_sample(from, (float)x * x_scale, (float)y * y_scale));
        }
    }
}
