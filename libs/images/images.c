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
    case PixelFormat_GrayScale:
        image.data.grayscale =
            calloc(sizeof(grayscale_pixel_t), width * height);
        return image;
    case PixelFormat_Rgb8:
        image.data.rgb8 = calloc(sizeof(rgb8_pixel_t), width * height);
        return image;
    case PixelFormat_Rgbf:
        image.data.rgbf = calloc(sizeof(rgbf_pixel_t), width * height);
        return image;
    }
}

void img_destroy(Image image) {
    switch (image.format) {
    case PixelFormat_GrayScale:
        free(image.data.grayscale);
        break;
    case PixelFormat_Rgb8:
        free(image.data.rgb8);
        break;
    case PixelFormat_Rgbf:
        free(image.data.rgbf);
        break;
    }
}

_Bool img_in_bound(Image *image, size_t x, size_t y) {
    return x < image->width && y < image->height;
}

ImageLoadResult img_load_file(
    char *filename, Image *image_out, bool detect_format,
    enum PixelFormat target_format
) {
    FILE *file = fopen(filename, "rb");

    uint8_t magic_header[3];
    size_t  read = fread(magic_header, sizeof(uint8_t), 3, file);
    if (read >= 2 && magic_header[0] == 'B' && magic_header[1] == 'M') {
        fseek(
            file, 0, SEEK_SET
        ); // Go back to the start of the file as expected by bmp_load_file
        enum BmpLoadResult bmp_result =
            bmp_load_file(file, image_out, detect_format, target_format);
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

size_t img_float_channels_length(enum PixelFormat format) {
    switch (format) {
    case PixelFormat_GrayScale:
        return 1;
    case PixelFormat_Rgbf:
        return 3;
    default:
        return 0;
    }
}
enum PixelFormat img_equivalent_float_channels_format(enum PixelFormat from) {
    switch (from) {
    case PixelFormat_Rgb8:
        return PixelFormat_Rgbf;
    case PixelFormat_GrayScale:
    case PixelFormat_Rgbf:
        return from;
    }
}

// Inline function definitions
grayscale_pixel_t img_some_to_grayscale(some_pixel_t value);
rgb8_pixel_t img_some_to_rgb8(some_pixel_t value);
rgbf_pixel_t img_some_to_rgbf(some_pixel_t value);
any_pixel_t img_some_to_any(some_pixel_t value, enum PixelFormat target);

any_pixel_t img_get_pixel_any(Image *image, size_t x, size_t y) {
    assert(img_in_bound(image, x, y));

    size_t linear_index = y * image->width + x;
    switch (image->format) {
    case PixelFormat_Rgb8:
        return (any_pixel_t){.rgb8 = image->data.rgb8[linear_index]};
    case PixelFormat_GrayScale:
        return (any_pixel_t){.grayscale = image->data.grayscale[linear_index]};
    case PixelFormat_Rgbf:
        return (any_pixel_t){.rgbf = image->data.rgbf[linear_index]};
    }
}
void img_set_pixel_any(
    Image *image, size_t x, size_t y, any_pixel_t new_value
) {
    assert(img_in_bound(image, x, y));

    size_t linear_index = y * image->width + x;
    switch (image->format) {
    case PixelFormat_Rgb8:
        image->data.rgb8[linear_index] = new_value.rgb8;
        break;
    case PixelFormat_GrayScale:
        image->data.grayscale[linear_index] = new_value.grayscale;
        break;
    case PixelFormat_Rgbf:
        image->data.rgbf[linear_index] = new_value.rgbf;
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
    img_set_pixel_any(image, x, y, img_some_to_any(new_value, image->format));
}

grayscale_pixel_t img_get_pixel_grayscale(Image *image, size_t x, size_t y) {
    return img_some_to_grayscale(img_get_pixel_some(image, x, y));
}
void img_set_pixel_grayscale(
    Image *image, size_t x, size_t y, grayscale_pixel_t new_value
) {
    img_set_pixel_any(
        image, x, y,
        img_some_to_any(
            (some_pixel_t
            ){.format = PixelFormat_GrayScale,
              .value  = {.grayscale = new_value}},
            image->format
        )
    );
}

rgb8_pixel_t img_get_pixel_rgb8(Image *image, size_t x, size_t y) {
    return img_some_to_rgb8(img_get_pixel_some(image, x, y));
}
void img_set_pixel_rgb8(
    Image *image, size_t x, size_t y, rgb8_pixel_t new_value
) {
    img_set_pixel_any(
        image, x, y,
        img_some_to_any(
            (some_pixel_t
            ){.format = PixelFormat_Rgb8, .value = {.rgb8 = new_value}},
            image->format
        )
    );
}

rgbf_pixel_t img_get_pixel_rgbf(Image *image, size_t x, size_t y) {
    return img_some_to_rgbf(img_get_pixel_some(image, x, y));
}
void img_set_pixel_rgbf(
    Image *image, size_t x, size_t y, rgbf_pixel_t new_value
) {
    img_set_pixel_any(
        image, x, y,
        img_some_to_any(
            (some_pixel_t
            ){.format = PixelFormat_Rgbf, .value = {.rgbf = new_value}},
            image->format
        )
    );
}
