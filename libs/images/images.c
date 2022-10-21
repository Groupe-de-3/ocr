#include "images.h"

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "img_formats/bmp.h"

Image img_new(size_t width, size_t height, enum PixelFormat format) {
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

any_pixel_t img_get_pixel_any(Image *image, size_t x, size_t y) {
    size_t linear_index = y * image->width + x;

    switch (image->format) {
    case ImageFormat_GrayScale:
        return (any_pixel_t){
            .grayscale = image->data.grayscale[linear_index],
        };
    case ImageFormat_Rgb8:
        return (any_pixel_t){
            .rgb8 = image->data.rgb8[linear_index],
        };
    }
}
void img_set_pixel_any(
    Image *image, size_t x, size_t y, any_pixel_t new_value
) {
    size_t linear_index = y * image->width + x;

    switch (image->format) {
    case ImageFormat_GrayScale:
        image->data.grayscale[linear_index] = new_value.grayscale;
        break;
    case ImageFormat_Rgb8:
        image->data.rgb8[linear_index] = new_value.rgb8;
        break;
    }
}

some_pixel_t img_get_pixel_some(Image *image, size_t x, size_t y) {
    return (some_pixel_t){
        .format = image->format,
        .value  = img_get_pixel_any(image, x, y),
    };
}
void img_set_pixel_some(
    Image *image, size_t x, size_t y, some_pixel_t new_value
) {
    assert(new_value.format == image->format);
    img_set_pixel_any(image, x, y, new_value.value);
}

grayscale_pixel_t img_get_pixel_grayscale(Image *image, size_t x, size_t y) {
    assert(img_in_bound(image, x, y));

    any_pixel_t pixel = img_get_pixel_any(image, x, y);

    switch (image->format) {
    case ImageFormat_GrayScale:
        return pixel.grayscale;
    case ImageFormat_Rgb8:
        return img_rbg8_to_grayscale(pixel.rgb8.r, pixel.rgb8.g, pixel.rgb8.b);
    }
}

void img_set_pixel_grayscale(
    Image *image, size_t x, size_t y, grayscale_pixel_t new_value
) {
    assert(img_in_bound(image, x, y));

    switch (image->format) {
    case ImageFormat_GrayScale:
        img_set_pixel_any(image, x, y, (any_pixel_t){.grayscale = new_value});
        break;
    case ImageFormat_Rgb8:
        {
            uint8_t rgb_value = img_grayscale_to_rgb8(new_value);
            img_set_pixel_any(
                image, x, y,
                (any_pixel_t){
                    .rgb8 =
                        (rgb8_pixel_t){
                                       .r = rgb_value,
                                       .g = rgb_value,
                                       .b = rgb_value,
                                       }
            }
            );
            break;
        }
    }
}

rgb8_pixel_t img_get_pixel_rgb8(Image *image, size_t x, size_t y) {
    assert(img_in_bound(image, x, y));

    any_pixel_t pixel = img_get_pixel_any(image, x, y);

    switch (image->format) {
    case ImageFormat_GrayScale:
        {
            uint8_t value = img_grayscale_to_rgb8(pixel.grayscale);
            return (rgb8_pixel_t){
                .r = value,
                .g = value,
                .b = value,
            };
        }
    case ImageFormat_Rgb8:
        return pixel.rgb8;
    }
}
void img_set_pixel_rgb8(
    Image *image, size_t x, size_t y, rgb8_pixel_t new_value
) {
    assert(img_in_bound(image, x, y));

    switch (image->format) {
    case ImageFormat_GrayScale:
        img_set_pixel_any(
            image, x, y,
            (any_pixel_t
            ){.grayscale =
                  img_rbg8_to_grayscale(new_value.r, new_value.g, new_value.b)}
        );
        break;
    case ImageFormat_Rgb8:
        img_set_pixel_any(image, x, y, (any_pixel_t){.rgb8 = new_value});
        break;
    }
}
