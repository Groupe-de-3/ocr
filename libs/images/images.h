#pragma once
#include <stdint.h>
#include <stdlib.h>

enum ImageType {
    ImageType_GrayScale,
};
typedef float grayscale_pixel_t;

typedef struct {
    enum ImageType type;
    union {
        grayscale_pixel_t *grayscale;
    } data;

    size_t width;
    size_t height;
} Image;

Image img_new(size_t width, size_t height, enum ImageType type);
void  img_destroy(Image image);

grayscale_pixel_t img_rbg8_to_grayscale(uint8_t r, uint8_t g, uint8_t b);
uint8_t           img_grayscale_to_rgb8(grayscale_pixel_t value);

grayscale_pixel_t img_get_pixel_grayscale(Image *image, size_t x, size_t y);
void              img_set_pixel_grayscale(
                 Image *image, size_t x, size_t y, grayscale_pixel_t new_value
             );
