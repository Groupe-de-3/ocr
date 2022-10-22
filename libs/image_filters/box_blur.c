#include "box_blur.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>

static void box_blur_horizontal(ImageView *in, ImageView *out, size_t box_size) {
    int half_size = (int)box_size / 2;

    enum PixelFormat target_format = img_equivalent_float_channels_format(in->image->format);
    size_t channel_length = img_float_channels_length(target_format);
    size_t channels_size = sizeof(float) * channel_length;
    
    for (int y = 0; y < in->height; y++) {
        // channel_length won't ever go above 4 or 5 so no stack problem.
        float new_value[channel_length];
        some_pixel_t out_pixel = { .format = target_format };

        memset(new_value, 0, channels_size);
        // Compute value of first horizontal pixel
        for (int x = -half_size; x <= half_size; x++) {
            any_pixel_t v = img_some_to_any(imgv_get_pixel_some(in, x, y), target_format);
            for (size_t c = 0; c < channel_length; c++)
                new_value[c] += v.float_channels[c];
        }
        
        memcpy(out_pixel.value.float_channels, new_value, channels_size);
        imgv_set_pixel_some(out, 0, y, out_pixel);
        
        for (int x = 1; x < in->width; x++) {
            any_pixel_t before_v = img_some_to_any(imgv_get_pixel_some(in, x - half_size - 1, y), target_format);
            for (size_t c = 0; c < channel_length; c++)
                new_value[c] -= before_v.float_channels[c];

            any_pixel_t after_v = img_some_to_any(imgv_get_pixel_some(in, x + half_size, y), target_format);
            for (size_t c = 0; c < channel_length; c++)
                new_value[c] += after_v.float_channels[c];

            memcpy(out_pixel.value.float_channels, new_value, channels_size);
            imgv_set_pixel_some(out, x, y, out_pixel);
        }
    }
}

static void box_blur_vertical(ImageView *in, ImageView *out, size_t box_size) {
    int half_size = (int)box_size / 2;

    enum PixelFormat target_format = img_equivalent_float_channels_format(in->image->format);
    size_t channel_length = img_float_channels_length(target_format);
    size_t channels_size = sizeof(float) * channel_length;

    for (int x = 0; x < in->width; x++) {
        // channel_length won't ever go above 4 or 5 so no stack problem.
        float new_value[channel_length];
        some_pixel_t out_pixel = { .format = target_format };

        memset(new_value, 0, channels_size);
        // Compute value of first vertical pixel
        for (int y = -half_size; y <= half_size; y++) {
            any_pixel_t v = img_some_to_any(imgv_get_pixel_some(in, x, y), target_format);
            for (size_t c = 0; c < channel_length; c++)
                new_value[c] += v.float_channels[c];
        }
        
        memcpy(out_pixel.value.float_channels, new_value, channels_size);
        imgv_set_pixel_some(out, x, 0, out_pixel);
        
        for (int y = 1; y < in->height; y++) {
            any_pixel_t above_v = img_some_to_any(imgv_get_pixel_some(in, x, y - half_size - 1), target_format);
            for (size_t c = 0; c < channel_length; c++)
                new_value[c] -= above_v.float_channels[c];

            any_pixel_t below_v = img_some_to_any(imgv_get_pixel_some(in, x, y + half_size), target_format);
            for (size_t c = 0; c < channel_length; c++)
                new_value[c] += below_v.float_channels[c];

            memcpy(out_pixel.value.float_channels, new_value, channels_size);
            imgv_set_pixel_some(out, x, y, out_pixel);
        }
    }
}


void box_blur_run(ImageView *in, ImageView *out, size_t box_size) {
    assert(out->width > 0 && out->height > 0);
    assert((box_size & 0x1) == 1);

    Image h_buffer = img_new((size_t)out->width, (size_t)out->height, out->image->format);
    ImageView h_view = imgv_default(&h_buffer);
    h_view.wraping_mode = WrappingMode_Clamp;

    box_blur_horizontal(in, &h_view, box_size);
    box_blur_vertical(&h_view, out, box_size);
    
    enum PixelFormat target_format = img_equivalent_float_channels_format(out->image->format);
    size_t channel_length = img_float_channels_length(target_format);
    for (int x = 0; x < out->width; x++)
        for (int y = 0; y < out->width; y++) {
            any_pixel_t v = imgv_get_pixel_any(out, x, y);
            for (size_t c = 0; c < channel_length; c++)
                v.float_channels[c] /= (float)box_size * (float)box_size;
            imgv_set_pixel_any(out, x, y, v);
        }
    
    img_destroy(h_buffer);
}
