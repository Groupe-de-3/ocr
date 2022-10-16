#include "images.h"

#include <assert.h>
#include <err.h>

Image img_new(size_t width, size_t height, enum ImageType type) {
    Image image = {
        .width  = width,
        .height = height,
        .type   = type,
    };

    switch (type) {
    case ImageType_GrayScale:
        image.data.grayscale = calloc(sizeof(float), width * height);
        return image;
    default:
        errx(1, "image_new: Invalid ImageType");
    }
}

void img_destroy(Image image) {
    switch (image.type) {
    case ImageType_GrayScale:
        free(image.data.grayscale);
        break;
    default:
        errx(1, "image_destroy: Invalid ImageType");
    }
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
    assert(x < image->width);
    assert(y < image->height);

    switch (image->type) {
    case ImageType_GrayScale:
        return image->data.grayscale[y * image->width + x];
    default:
        errx(1, "img_get_pixel_grayscale: Invalid ImageType");
    }
}

void img_set_pixel_grayscale(
    Image *image, size_t x, size_t y, grayscale_pixel_t new_value
) {
    assert(x < image->width);
    assert(y < image->height);

    switch (image->type) {
    case ImageType_GrayScale:
        image->data.grayscale[y * image->width + x] = new_value;
    default:
        errx(1, "img_get_pixel_grayscale: Invalid ImageType");
    }
}
