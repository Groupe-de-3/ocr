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

any_pixel_t img_some_to_any(some_pixel_t value, enum PixelFormat target) {
    switch (target) {
    case PixelFormat_Rgb8:
        return (any_pixel_t){.rgb8 = img_some_to_rgb8(value)};
    case PixelFormat_GrayScale:
        return (any_pixel_t){.grayscale = img_some_to_grayscale(value)};
    case PixelFormat_Rgbf:
        return (any_pixel_t){.rgbf = img_some_to_rgbf(value)};
    }
}
grayscale_pixel_t img_some_to_grayscale(some_pixel_t value) {
    switch (value.format) {
    case PixelFormat_Rgb8:
        return (grayscale_pixel_t
        ){0.299f * ((float)value.value.rgb8.r / 255.f) +
          0.587f * ((float)value.value.rgb8.g / 255.f) +
          0.114f * ((float)value.value.rgb8.b / 255.f)};
    case PixelFormat_GrayScale:
        return value.value.grayscale;
    case PixelFormat_Rgbf:
        return (grayscale_pixel_t
        ){0.299f * value.value.rgbf.r + 0.587f * value.value.rgbf.g +
          0.114f * value.value.rgbf.b};
    }
}
rgb8_pixel_t img_some_to_rgb8(some_pixel_t value) {
    switch (value.format) {
    case PixelFormat_Rgb8:
        return value.value.rgb8;
    case PixelFormat_GrayScale:
        {
            uint8_t v =
                (uint8_t)(value.value.grayscale * (grayscale_pixel_t)255);
            return (rgb8_pixel_t){v, v, v};
        }
    case PixelFormat_Rgbf:
        return (rgb8_pixel_t){
            (uint8_t)(value.value.rgbf.r * 255.f),
            (uint8_t)(value.value.rgbf.g * 255.f),
            (uint8_t)(value.value.rgbf.b * 255.f),
        };
    }
}
rgbf_pixel_t img_some_to_rgbf(some_pixel_t value) {
    switch (value.format) {
    case PixelFormat_Rgb8:
        return (rgbf_pixel_t){
            ((float)value.value.rgb8.r / 255.f),
            ((float)value.value.rgb8.g / 255.f),
            ((float)value.value.rgb8.b / 255.f),
        };
    case PixelFormat_GrayScale:
        return (rgbf_pixel_t
        ){value.value.grayscale, value.value.grayscale, value.value.grayscale};
    case PixelFormat_Rgbf:
        return value.value.rgbf;
    }
}

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
