#include "images.h"

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "img_formats/bmp.h"

Image img_new(size_t width, size_t height, enum ImageFormat format) {
    Image image = {
        .width  = width,
        .height = height,
        .format = format,
    };

    switch (format) {
    case ImageFormat_GrayScale:
        image.data.grayscale =
            calloc(sizeof(grayscale_pixel_t), width * height);
        return image;
    case ImageFormat_Rgb8:
        image.data.rgb8 = calloc(sizeof(rgb8_pixel_t), width * height);
        return image;
    }
}

void img_destroy(Image image) {
    switch (image.format) {
    case ImageFormat_GrayScale:
        free(image.data.grayscale);
        break;
    case ImageFormat_Rgb8:
        free(image.data.rgb8);
        break;
    }
}

_Bool img_in_bound(Image *image, size_t x, size_t y) {
    return x < image->width && y < image->height;
}

ImageLoadResult img_load_file(char *filename, Image *image_out) {
    FILE *file = fopen(filename, "rb");

    uint8_t magic_header[3];
    size_t  read = fread(magic_header, sizeof(uint8_t), 3, file);
    if (read >= 2 && magic_header[0] == 'B' && magic_header[1] == 'M') {
        fseek(
            file, 0, SEEK_SET
        ); // Go back to the start of the file as expected by bmp_load_file
        enum BmpLoadResult bmp_result = bmp_load_file(file, image_out);
        fclose(file);
        return (ImageLoadResult){
            .success = bmp_result == BmpLoadResult_Ok,
        };
    }

    fclose(file);
    return (ImageLoadResult){
        .success = false,
    };
}

grayscale_pixel_t img_rbg8_to_grayscale(uint8_t r, uint8_t g, uint8_t b) {
    float gray_scale = 0.299f * ((float)r / 255.f) +
                       0.587f * ((float)g / 255.f) +
                       0.114f * ((float)b / 255.f);
    // Only if grayscale_pixel_t is a float
    return gray_scale;
}
uint8_t img_grayscale_to_rgb8(grayscale_pixel_t value) {
    return (uint8_t)(value * 255.f);
}

grayscale_pixel_t img_get_pixel_grayscale(Image *image, size_t x, size_t y) {
    assert(img_in_bound(image, x, y));

    size_t linear_index = y * image->width + x;

    switch (image->format) {
    case ImageFormat_GrayScale:
        return image->data.grayscale[linear_index];
    case ImageFormat_Rgb8:
        {
            rgb8_pixel_t pixel = image->data.rgb8[linear_index];
            return img_rbg8_to_grayscale(pixel.r, pixel.g, pixel.b);
        }
    }
}

void img_set_pixel_grayscale(
    Image *image, size_t x, size_t y, grayscale_pixel_t new_value
) {
    assert(img_in_bound(image, x, y));

    size_t linear_index = y * image->width + x;

    switch (image->format) {
    case ImageFormat_GrayScale:
        image->data.grayscale[linear_index] = new_value;
        break;
    case ImageFormat_Rgb8:
        {
            uint8_t rgb_value              = img_grayscale_to_rgb8(new_value);
            image->data.rgb8[linear_index] = (rgb8_pixel_t){
                .r = rgb_value,
                .g = rgb_value,
                .b = rgb_value,
            };
            break;
        }
    }
}

rgb8_pixel_t img_get_pixel_rgb8(Image *image, size_t x, size_t y) {
    assert(img_in_bound(image, x, y));

    size_t linear_index = y * image->width + x;

    switch (image->format) {
    case ImageFormat_GrayScale:
        {
            uint8_t value =
                img_grayscale_to_rgb8(image->data.grayscale[linear_index]);
            return (rgb8_pixel_t){
                .r = value,
                .g = value,
                .b = value,
            };
        }
    case ImageFormat_Rgb8:
        return image->data.rgb8[linear_index];
    }
}
void img_set_pixel_rgb8(
    Image *image, size_t x, size_t y, rgb8_pixel_t new_value
) {
    assert(img_in_bound(image, x, y));

    size_t linear_index = y * image->width + x;

    switch (image->format) {
    case ImageFormat_GrayScale:
        image->data.grayscale[linear_index] =
            img_rbg8_to_grayscale(new_value.r, new_value.g, new_value.b);
        break;
    case ImageFormat_Rgb8:
        image->data.rgb8[linear_index] = new_value;
        break;
    }
}
